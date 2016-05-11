#include "Stack.h"
#include <stdlib.h>

ElaraStack *elara_stack_create() {
    ElaraStack *stack = malloc(sizeof(ElaraStack));
    SLIST_INIT(stack);
    return stack;
}

void elara_stack_insert(ElaraStack *stack, void *entry) {
    ElaraStackEntry *stack_entry = malloc(sizeof(ElaraStackEntry));
    stack_entry->data = entry;
    SLIST_INSERT_HEAD(stack, stack_entry, entries);
}

int elara_stack_count(ElaraStack *stack) {
    __block int count = 0;
    elara_stack_foreach(stack, ^(void *entry) {
        count += 1;
    });
    return count;
}

void *elara_stack_pop(ElaraStack *stack) {
    ElaraStackEntry *stack_entry = SLIST_FIRST(stack);
    void *entry = stack_entry->data;
    SLIST_REMOVE_HEAD(stack, entries);
    free(stack_entry);
    return entry;
}

void *elara_stack_get(ElaraStack *stack, int index) {
    ElaraStackEntry *entry = SLIST_FIRST(stack);
    for (int i = 0; i < index; i++) {
        entry = SLIST_NEXT(entry, entries);
    }
    return entry->data;
}

void elara_stack_foreach(ElaraStack *stack, ElaraStackEntryBlock block) {
    ElaraStackEntry *entry;
    SLIST_FOREACH(entry, stack, entries) {
        block(entry->data);
    }
}

void elara_stack_dealloc(ElaraStack *stack, ElaraStackEntryBlock entry_dealloc_block) {
    while (!SLIST_EMPTY(stack)) {
        ElaraStackEntry *stack_entry = SLIST_FIRST(stack);
        entry_dealloc_block(stack_entry->data);
        SLIST_REMOVE_HEAD(stack, entries);
        free(stack_entry);
    }
    free(stack);
}
