#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Block.h>

#include "TestContext.h"

void testContext_set_parent_focused_children(TestContext *testContext) {
    TestContext *current = testContext;
    while (current != NULL) {
        current->has_focused_children = elara_true;
        current = current->parent;
        if (current == NULL || current->has_focused_children == elara_true) {
            break;
        }
    }
}

TestContext *testContext_create(TestContext *parent_context, TestFocus focus) {
    TestContext *context = calloc(1, sizeof(TestContext));
    context->name = NULL;
    context->block = NULL;
    context->status = TestStatusNotATest;
    context->beforeEach = elara_list_create();
    context->afterEach = elara_list_create();
    context->message = NULL;

    context->parent = parent_context;
    context->children = elara_list_create();
    context->focus = focus;
    context->has_focused_children = focus == TestFocusFocused ? elara_true : elara_false;

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

// Not Safe
void prepend(char *string, const char *to_prepend) {
    size_t len = strlen(to_prepend);

    memmove(string + len, string, strlen(string) + 1);

    for (size_t i = 0; i < len; i++) {
        string[i] = to_prepend[i];
    }
}

char *testContext_full_test_name(TestContext *context) {
    TestContext *current = context;
    size_t required_length = 0;
    while (1) {
        required_length += strlen(current->name);
        if (current->parent != NULL && current->parent->name != NULL) {
            required_length += 1;
            current = current->parent;
        } else {
            required_length += 1;
            break;
        }
    }
    current = context->parent;
    char *name = calloc(1, required_length);
    while (current != NULL && current->name != NULL) {
        prepend(name, " ");
        prepend(name, current->name);
        current = current->parent;
    }
    strncat(name, context->name, required_length);
    return name;
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
