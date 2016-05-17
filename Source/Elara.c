#include <sys/queue.h>
#include <stdio.h>
#include <Block.h>

#include "Elara.h"
#include "TestContext.h"

static TestContext *baseContext = NULL;
TestContext *currentContext = NULL;

void describe(const char *name, ElaraTestBlock block) {
    if (baseContext == NULL) {
        baseContext = testContext_create(NULL);
        currentContext = baseContext;
    }

    TestContext *context = testContext_create(currentContext);
    context->name = (char *)name;
    TestContext *oldContext = currentContext;
    currentContext = context;
    if (block) {
        context->block = Block_copy(block);
        context->block();
    }
    currentContext = oldContext;
}

void beforeEach(ElaraTestBlock before) {
    elara_list_insert(currentContext->beforeEach, Block_copy(before));
}

void afterEach(ElaraTestBlock after) {
    elara_list_insert(currentContext->afterEach, Block_copy(after));
}

void it(const char *name, ElaraTestBlock test) {
    TestContext *itContext = testContext_create(currentContext);

    itContext->name = (char *)name;
    itContext->status = TestStatusNotRun;
    if (test) {
        itContext->block = Block_copy(test);
    }
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

int run(TestContext *context) {
    __block int returnValue = 0;
    TestContext *oldContext = currentContext;
    currentContext = context;
    if (context->status == TestStatusNotATest) {
        elara_list_foreach(context->children, ^(void *entry){
            TestContext *childContext = (TestContext *)entry;
            returnValue += run(childContext);
        });
    } else if (context->status == TestStatusNotRun) {
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
    int result = run(currentContext);
    printf("\n");
    return result;
}

