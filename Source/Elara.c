#include <sys/queue.h>
#include <stdio.h>

#include "Elara.h"
#include "TestContext.h"

TestContext *baseContext = NULL;
TestContext *currentContext = NULL;

void elara_tests(ElaraTestBlock tests) {
    if (baseContext == NULL) {
        baseContext = testContext_create(NULL);
    }
    currentContext = baseContext;
}

void describe(const char *name, ElaraTestBlock block) {
    TestContext *context = testContext_create(currentContext);
    context->name = (char *)name;
    context->block = block;
}

void it(const char *name, ElaraTestBlock test) {
    TestContext *itContext = testContext_create(currentContext);

    itContext->name = (char *)name;
    itContext->block = test;
    itContext->status = TestStatusNotRun;
}

void expect(int condition) {
    if (condition) {
        currentContext->status = TestStatusSucceeded;
    } else {
        currentContext->status = TestStatusFailed;
    }
}

void run(TestContext *context) {
    TestContext *oldContext = currentContext;
    if (context->block != NULL) {
        currentContext = context;
        context->block();
        elara_stack_foreach(context->children, ^(void *entry){
            TestContext *childContext = (TestContext *)entry;
            run(childContext);
        });
        currentContext = oldContext;
    }
}

