#include "TestContext.h"

TestContext *testContext_create(TestContext *parent_context) {
    TestContext *context = malloc(sizeof(TestContext));
    context->children = elara_stack_create();

    if (parent_context) {
        elara_stack_insert(parent_context->children, context);
    }

    return context
}

void testContext_dealloc(TestContext *context) {
    elara_stack_dealloc(context->children, ^(TestContext *subcontext){
        testContext_dealloc(subcontext);
    });
    free(context);
}

