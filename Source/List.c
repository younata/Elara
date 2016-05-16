#include "List.h"
#include <stdlib.h>

ElaraList *elara_list_create() {
    ElaraList *list = malloc(sizeof(ElaraList));
    LIST_INIT(list);
    return list;
}

ElaraListEntry *elara_list_get_entry(ElaraList *list, int number) {
    ElaraListEntry *entry = LIST_FIRST(list);

    int index = 0;
    while (index < number) {
        entry = LIST_NEXT(entry, entries);
        index += 1;
    }

    return entry;
}

void elara_list_insert(ElaraList *list, void *entry) {
    ElaraListEntry *list_entry = malloc(sizeof(ElaraListEntry));
    list_entry->data = entry;

    int length = elara_list_count(list);
    if (length == 0) {
        LIST_INSERT_HEAD(list, list_entry, entries);
    } else {
        ElaraListEntry *last_entry = elara_list_get_entry(list, length - 1);
    
        LIST_INSERT_AFTER(last_entry, list_entry, entries);
    }
}

int elara_list_count(ElaraList *list) {
    __block int count = 0;
    elara_list_foreach(list, ^(void *entry) {
        count += 1;
    });
    return count;
}

void *elara_list_pop(ElaraList *list) {
    int length = elara_list_count(list);
    if (length == 0) {
        return NULL;
    }
    ElaraListEntry *list_entry = elara_list_get_entry(list, length - 1);
    void *entry = list_entry->data;
    LIST_REMOVE(list_entry, entries);
    free(list_entry);
    return entry;
}

void *elara_list_get(ElaraList *list, int index) {
    ElaraListEntry *entry = LIST_FIRST(list);
    for (int i = 0; i < index; i++) {
        entry = LIST_NEXT(entry, entries);
    }
    if (entry) {
        return entry->data;
    }
    return entry;
}

void elara_list_foreach(ElaraList *list, ElaraListEntryBlock block) {
    ElaraListEntry *entry;
    LIST_FOREACH(entry, list, entries) {
        block(entry->data);
    }
}

void elara_list_dealloc(ElaraList *list, ElaraListEntryBlock entry_dealloc_block) {
    while (!LIST_EMPTY(list)) {
        ElaraListEntry *list_entry = LIST_FIRST(list);
        entry_dealloc_block(list_entry->data);
        LIST_REMOVE(list_entry, entries);
        free(list_entry);
    }
    free(list);
}
