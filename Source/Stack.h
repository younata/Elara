#ifndef __ELARA_STACK_H__
#define __ELARA_STACK_H__

#include <sys/queue.h>

typedef void (^ElaraStackEntryBlock)(void *);

SLIST_HEAD(Elara_Stack_Head, Elara_Stack_Entry);
typedef struct Elara_Stack_Head ElaraStack;

struct Elara_Stack_Entry {
    void *data;
    SLIST_ENTRY(Elara_Stack_Entry) entries;
};
typedef struct Elara_Stack_Entry ElaraStackEntry;

ElaraStack *elara_stack_create();
void elara_stack_insert(ElaraStack *stack, void *entry);
int elara_stack_count(ElaraStack *stack);
void *elara_stack_pop(ElaraStack *stack);
void *elara_stack_get(ElaraStack *stack, int index);
void elara_stack_foreach(ElaraStack *stack, ElaraStackEntryBlock block);
void elara_stack_dealloc(ElaraStack *stack, ElaraStackEntryBlock entry_dealloc_block);

#endif /* __ELARA_STACK_H__ */
