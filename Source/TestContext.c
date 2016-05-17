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
    context->beforeEach = elara_list_create();
    context->afterEach = NULL;

    context->parent = parent_context;
    context->children = elara_list_create();

    if (parent_context) {
        elara_list_insert(parent_context->children, context);
    }

    return context;
}

void testContext_run_beforeEachs(TestContext *context) {
    TestContext *ctx = context;
    ElaraList *beforeEachs = elara_list_create();
    while (ctx->parent != NULL) {
        elara_list_foreach(ctx->beforeEach, ^(void *entry){
            elara_list_insert(beforeEachs, entry);
        });
        ctx = ctx->parent;
    }

    elara_list_foreach(beforeEachs, ^(void *entry){
        ElaraTestBlock beforeEach = (ElaraTestBlock)entry;
        beforeEach();
    });

    elara_list_dealloc(beforeEachs, ^(void *blah){});
}

void testContext_dealloc(TestContext *context) {
    elara_list_dealloc(context->children, ^(void *entry) {
        TestContext *subcontext = (TestContext *)entry;
        testContext_dealloc(subcontext);
    });

    elara_list_dealloc(context->beforeEach, ^(void *entry) {
        Block_release((ElaraTestBlock)entry);
    });

    if (context->block != NULL) {
        Block_release(context->block);
        context->block = NULL;
    }

    free(context);
}

