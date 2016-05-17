#include <stdlib.h>
#include <Block.h>
#include <string.h>
#include <stdio.h>

#include "TestContext.h"

void testContext_run_list_of_blocks(TestContext *context, ElaraList *(^blockListGetter)(TestContext *ctx));

TestContext *testContext_create(TestContext *parent_context) {
    TestContext *context = malloc(sizeof(TestContext));
    context->name = NULL;
    context->block = NULL;
    context->status = TestStatusNotATest;
    context->beforeEach = elara_list_create();
    context->afterEach = elara_list_create();

    context->parent = parent_context;
    context->children = elara_list_create();

    if (parent_context) {
        elara_list_insert(parent_context->children, context);
    }

    return context;
}

void testContext_run_beforeEachs(TestContext *context) {
    testContext_run_list_of_blocks(context, ^ElaraList *(TestContext *ctx) { return ctx->beforeEach; });
}

void testContext_run_afterEachs(TestContext *context) {
    testContext_run_list_of_blocks(context, ^ElaraList *(TestContext *ctx) { return ctx->afterEach; });
}

void testContext_run_list_of_blocks(TestContext *context, ElaraList *(^blockListGetter)(TestContext *ctx)) {
    TestContext *ctx = context;
    ElaraList *blocks = elara_list_create();
    while (ctx->parent != NULL) {
        elara_list_foreach(blockListGetter(ctx), ^(void *entry){
            elara_list_insert(blocks, entry);
        });
        ctx = ctx->parent;
    }

    elara_list_foreach(blocks, ^(void *entry){
        ElaraTestBlock block = (ElaraTestBlock)entry;
        block();
    });

    elara_list_dealloc(blocks, ^(void *blah){});
}

void testContext_dealloc(TestContext *context) {
    elara_list_dealloc(context->children, ^(void *entry) {
        TestContext *subcontext = (TestContext *)entry;
        testContext_dealloc(subcontext);
    });

    elara_list_dealloc(context->beforeEach, ^(void *entry) {
        Block_release((ElaraTestBlock)entry);
    });

    elara_list_dealloc(context->afterEach, ^(void *entry) {
        Block_release((ElaraTestBlock)entry);
    });

    if (context->block != NULL) {
        Block_release(context->block);
        context->block = NULL;
    }

    free(context);
}

