#ifndef __ELARA_LIST_H__
#define __ELARA_LIST_H__

#include <sys/queue.h>

typedef void (^ElaraListEntryBlock)(void *);

LIST_HEAD(Elara_List_Head, Elara_List_Entry);
typedef struct Elara_List_Head ElaraList;

struct Elara_List_Entry {
    void *data;
    LIST_ENTRY(Elara_List_Entry) entries;
};
typedef struct Elara_List_Entry ElaraListEntry;

ElaraList *elara_list_create();
void elara_list_append(ElaraList *list, void *entry);
void elara_list_insert(ElaraList *list, void *entry, int position);
void elara_list_insert_list(ElaraList *initial, ElaraList *other, int position);
int elara_list_count(ElaraList *list);
void *elara_list_pop(ElaraList *list);
void *elara_list_get(ElaraList *list, int index);
void elara_list_foreach(ElaraList *list, ElaraListEntryBlock block);
void elara_list_dealloc(ElaraList *list, ElaraListEntryBlock entry_dealloc_block);

#endif /* __ELARA_LIST_H__ */
