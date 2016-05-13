#include <stdlib.h>
#include <Block.h>
#include <string.h>
#include <stdio.h>

#include "TestContext.h"

TestContext *testContext_create(TestContext *parent_context) {
    TestContext *context = malloc(sizeof(TestContext));
    context->name = NULL;
    context->block = NULL;
    context->status = TestStatusNotATest;
    context->beforeEach = NULL;
    context->afterEach = NULL;

    context->children = elara_stack_create();

    if (parent_context) {
        elara_stack_insert(parent_context->children, context);
    }

    return context;
}

void testContext_dealloc(TestContext *context) {
    elara_stack_dealloc(context->children, ^(void *entry) {
        TestContext *subcontext = (TestContext *)entry;
        testContext_dealloc(subcontext);
    });

    if (context->block != NULL) {
        Block_release(context->block);
        context->block = NULL;
    }

    free(context);
}

