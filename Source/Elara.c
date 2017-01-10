#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Block.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <execinfo.h>

#include "Elara.h"
#include "ElaraList.h"
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
    if (block != NULL) {
        context->block = Block_copy(block);
        context->block();
    }
    currentContext = oldContext;
}

void create_it(const char *name, ElaraTestBlock test, TestFocus focus) {
    TestContext *itContext = testContext_create(currentContext, focus);

    itContext->name = (char *)name;
    itContext->status = TestStatusNotRun;
    if (test != NULL) {
        itContext->block = Block_copy(test);
    }
}

void elara_assert(ElaraTestResult passed, char *message, const char *file, int line_number) {
    if (currentContext->status == TestStatusFailed || currentContext->status == TestStatusErrored) { return; }
    switch (passed) {
    case ElaraTestResultPass:
        currentContext->status = TestStatusSucceeded;
        break;
    case ElaraTestResultFail: {
        currentContext->status = TestStatusFailed;
        size_t message_length = strlen(message);
        if (currentContext->message != NULL) {
            free(currentContext->message);
            currentContext->message = NULL;
        }
        currentContext->message = (char *)calloc(1, message_length);
        strncpy(currentContext->message, message, message_length);

        printf("\n%s (%s:%d) Failed: %s\n", testContext_full_test_name(currentContext), file, line_number, message);
        break;
    }
    case ElaraTestResultError: {
        currentContext->status = TestStatusErrored;
        size_t message_length = strlen(message);
        if (currentContext->message != NULL) {
            free(currentContext->message);
            currentContext->message = NULL;
        }
        currentContext->message = (char *)calloc(1, message_length);
        strncpy(currentContext->message, message, message_length);

        printf("\n%s (%s:%d) Errored: %s\n", testContext_full_test_name(currentContext), file, line_number, message);
        break;
    }
    }
    if (message != NULL) {
        free(message);
    }
}

// public

void describe(const char *name, ElaraTestBlock block) {
    create_describe(name, block, TestFocusUnfocused);
}

void fdescribe(const char *name, ElaraTestBlock block) {
    create_describe(name, block, TestFocusFocused);
}

void xdescribe(const char *name, ElaraTestBlock block) {
    create_describe(name, block, TestFocusSkipped);
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

void xit(const char *name, ElaraTestBlock test) {
    create_it(name, test, TestFocusSkipped);
}

int run(TestContext *context, TestFocus focus, ElaraList *results) {
    __block int returnValue = 0;
    TestContext *oldContext = currentContext;
    currentContext = context;
    if (context->has_focused_children) {
        focus = TestFocusFocused;
    }
    if (context->status == TestStatusNotATest) {
        elara_list_foreach(context->children, ^(void *entry){
            TestContext *childContext = (TestContext *)entry;
            TestFocus currentFocus = focus;
            if (focus == TestFocusFocused || childContext->has_focused_children) {
                currentFocus = TestFocusFocused;
            }
            returnValue += run(childContext, currentFocus, results);
        });
    } else if (context->status == TestStatusNotRun) {
        if (context->block == NULL || focus != context->focus) {
            testReport_add_report(results, testContext_full_test_name(context), NULL, TestStatusSkipped, 0);
            printf("*");
            fflush(stdout);
            return 0;
        }
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
        testReport_add_report(results, testContext_full_test_name(context), context->message, context->status, runtime);
    }
    currentContext = oldContext;
    return returnValue;
}

void segfault_handler(int signal, siginfo_t *si, void *arg) {
    void *callstack[128];

    int frames = backtrace(callstack, 128);

    fprintf(stderr, "Segfault caught at %p, exiting\n\n", si->si_addr);

    backtrace_symbols_fd(callstack, frames, STDERR_FILENO);
    exit(-1);
}

int elara_main(int argc, char *argv[]) {
    FILE *xunit_output = NULL;
    FILE *rspec_output = NULL;

    setElaraEnvironmentAssert(elara_assert);

    // install a segfault handler
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = segfault_handler;
    sa.sa_flags   = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);

    // handle command line options
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
    int result = 0;
    if (currentContext != NULL) {
        result = run(currentContext, TestFocusUnfocused, reports);
    } else {
        printf("Elara: No Tests Found");
    }
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
