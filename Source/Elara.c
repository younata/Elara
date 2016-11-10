#include <sys/queue.h>
#include <stdio.h>
#include <Block.h>

#include "Elara.h"
#include "TestContext.h"

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
        printf("\n%s (%s-%d) Failed.\nExpected '%s' to be truthy, got falsy\n", currentContext->name, file, line_number, expression);
    }
}

int run(TestContext *context, TestFocus focus) {
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
            returnValue += run(childContext, currentFocus);
        });
    } else if (context->status == TestStatusNotRun && focus == context->focus) {
        testContext_run_beforeEachs(context);
        context->block();
        testContext_run_afterEachs(context);
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
    }
    currentContext = oldContext;
    return returnValue;
}

int elara_main(int argc, char *argv[]) {
    int result = run(currentContext, TestFocusUnfocused);
    printf("\n");
    return result;
}
