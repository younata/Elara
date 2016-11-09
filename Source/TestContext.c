#include <stdlib.h>
#include <Block.h>
#include <string.h>

#include "TestContext.h"

void testContext_set_parent_focused_children(TestContext *testContext) {
    TestContext *current = testContext;
    while (current->parent != NULL) {
        current = current->parent;
        if (current->has_focused_children == elara_true) {
            break;
        }
        current->has_focused_children = elara_true;
    }
}

TestContext *testContext_create(TestContext *parent_context, TestFocus focus) {
    TestContext *context = calloc(1, sizeof(TestContext));
    context->name = NULL;
    context->block = NULL;
    context->status = TestStatusNotATest;
    context->beforeEach = elara_list_create();
    context->afterEach = elara_list_create();

    context->parent = parent_context;
    context->children = elara_list_create();
    context->focus = focus;
    context->has_focused_children = elara_false;

    if (parent_context) {
        if (parent_context->focus == TestFocusFocused) {
            context->focus = TestFocusFocused;
        } else if (parent_context->focus == TestFocusUnfocused && focus == TestFocusFocused) {
            testContext_set_parent_focused_children(context);
        }
        elara_list_append(parent_context->children, context);
    }

    return context;
}

void testContext_run_beforeEachs(TestContext *context) {
    TestContext *ctx = context;
    ElaraList *blocks_list = elara_list_create();
    do {
        elara_list_insert_list(blocks_list, ctx->beforeEach, 0);
        ctx = ctx->parent;
    } while (ctx != NULL && ctx->parent != NULL);

    elara_list_foreach(blocks_list, ^(void *entry) {
        ElaraTestBlock block = (ElaraTestBlock)entry;
        block();
    });

    elara_list_dealloc(blocks_list, ^(void *entry){});
}

void testContext_run_afterEachs(TestContext *context) {
    TestContext *ctx = context;
    ElaraList *blocks_list = elara_list_create();
    do {
        elara_list_insert_list(blocks_list, ctx->afterEach, -1);
        ctx = ctx->parent;
    } while (ctx != NULL && ctx->parent != NULL);

    elara_list_foreach(blocks_list, ^(void *entry) {
        ElaraTestBlock block = (ElaraTestBlock)entry;
        block();
    });

    elara_list_dealloc(blocks_list, ^(void *entry){});
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

