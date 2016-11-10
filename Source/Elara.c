#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Block.h>
#include <unistd.h>
#include <time.h>

#include "Elara.h"
#include "List.h"
#include "TestContext.h"
#include "TestReporter.h"

// private

static TestContext *baseContext = NULL;
TestContext *currentContext = NULL;

void create_describe(const char *name, ElaraTestBlock block, TestFocus focus) {
    if (baseContext == NULL) {
        baseContext = testContext_create(NULL, TestFocusUnfocused);
        currentContext = baseContext;
    }

    TestContext *context = testContext_create(currentContext, focus);
    context->name = (char *)name;
    TestContext *oldContext = currentContext;
    currentContext = context;
    if (block) {
        context->block = Block_copy(block);
        context->block();
    }
    currentContext = oldContext;
}

void create_it(const char *name, ElaraTestBlock test, TestFocus focus) {
    TestContext *itContext = testContext_create(currentContext, focus);

    itContext->name = (char *)name;
    itContext->status = TestStatusNotRun;
    if (test) {
        itContext->block = Block_copy(test);
    }
}

// public

void describe(const char *name, ElaraTestBlock block) {
    create_describe(name, block, TestFocusUnfocused);
}

void fdescribe(const char *name, ElaraTestBlock block) {
    create_describe(name, block, TestFocusFocused);
}

void beforeEach(ElaraTestBlock before) {
    elara_list_append(currentContext->beforeEach, Block_copy(before));
}

void afterEach(ElaraTestBlock after) {
    elara_list_append(currentContext->afterEach, Block_copy(after));
}

void it(const char *name, ElaraTestBlock test) {
    create_it(name, test, TestFocusUnfocused);
}

void fit(const char *name, ElaraTestBlock test) {
    create_it(name, test, TestFocusFocused);
}

void elara_expect(int condition, const char *expression, const char *file, int line_number) {
    if (currentContext->status == TestStatusFailed) { return; }
    if (condition) {
        currentContext->status = TestStatusSucceeded;
    } else {
        currentContext->status = TestStatusFailed;
        size_t expected_length = strlen(expression) + 38;
        if (currentContext->message != NULL) {
            free(currentContext->message);
            currentContext->message = NULL;
        }
        currentContext->message = (char *)calloc(1, expected_length);
        snprintf(currentContext->message, expected_length, "Expected '%s' to be truthy, got falsy", expression);

        printf("\n%s (%s:%d) Failed: Expected '%s' to be truthy, got falsy\n", currentContext->name, file, line_number, expression);
    }
}

int run(TestContext *context, TestFocus focus, ElaraList *results) {
    __block int returnValue = 0;
    TestContext *oldContext = currentContext;
    currentContext = context;
    if (context->status == TestStatusNotATest) {
        elara_list_foreach(context->children, ^(void *entry){
            TestContext *childContext = (TestContext *)entry;
            TestFocus currentFocus = focus;
            if (focus == TestFocusFocused || childContext->has_focused_children) {
                currentFocus = TestFocusFocused;
            }
            returnValue += run(childContext, currentFocus, results);
        });
    } else if (context->status == TestStatusNotRun && focus == context->focus) {
        clock_t start = clock(), diff;
        testContext_run_beforeEachs(context);
        context->block();
        testContext_run_afterEachs(context);
        diff = clock() - start;
        double runtime = (double)diff / (double) CLOCKS_PER_SEC;
        returnValue = 1;
        switch (context->status) {
            case TestStatusFailed:
                printf("F");
                break;
            case TestStatusErrored:
                printf("X");
                break;
            case TestStatusSucceeded:
                printf(".");
                returnValue = 0;
                break;
            default:
                break;
        }
        fflush(stdout);
        testReport_add_report(results, context->name, context->message, context->status, runtime);
    }
    currentContext = oldContext;
    return returnValue;
}

int elara_main(int argc, char *argv[]) {
    FILE *xunit_output = NULL;
    FILE *rspec_output = NULL;

    int option = -1;

    while ((option = getopt(argc, argv, "hr:x:")) != -1) {
        switch (option) {
        case 'h':
            printf("Usage: %s [-r rspec_style_output_path] [-x xunit_style_output_path]\n", argv[0]);
            return 0;
        case 'r':
            if (strncmp(optarg, "-", strlen(optarg)) == 0) {
                rspec_output = stdout;
            } else {
                rspec_output = fopen(optarg, "a");
            }
            break;
        case 'x':
            if (strncmp(optarg, "-", strlen(optarg)) == 0) {
                xunit_output = stdout;
            } else {
                xunit_output = fopen(optarg, "a");
            }
            break;
        default:
            break;
        }
    }

    ElaraList *reports = elara_list_create();
    int result = run(currentContext, TestFocusUnfocused, reports);
    printf("\n");
    if (xunit_output != NULL) {
        testReport_create_report(reports, TestReportStyleXUnit, xunit_output);
    }
    if (rspec_output != NULL) {
        testReport_create_report(reports, TestReportStyleRSpec, rspec_output);
    }

    elara_list_dealloc(reports, ^(void *object) {
        free(object);
    });
    if (xunit_output != NULL && xunit_output != stdout) {
        fclose(xunit_output);
    }
    if (rspec_output != NULL && rspec_output != stdout) {
        fclose(rspec_output);
    }
    return result;
}
