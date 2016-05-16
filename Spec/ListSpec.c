#include <stdio.h>
#include <sys/queue.h>

#include "Elara.h"
#include "List.h"

void ListSpec() {
    describe("An ElaraList", ^{
        __block ElaraList *subject;

        it("starts off with 0 objects", ^{
            subject = elara_list_create();

            expect(LIST_EMPTY(subject));
            expect(elara_list_count(subject) == 0);

            elara_list_dealloc(subject, ^(void *entry){});
        });

        it("allows an object to be inserted easily", ^{
            subject = elara_list_create();

            int number = 1;
            elara_list_insert(subject, &number);
            expect(elara_list_count(subject) == 1);
            expect(*((int *)(LIST_FIRST(subject)->data)) == number);

            int other_number = 2;
            elara_list_insert(subject, &other_number);
            expect(elara_list_count(subject) == 2);
            ElaraListEntry *first_entry = LIST_FIRST(subject);
            expect(*((int *)(first_entry->data)) == number);

            expect(*((int *)(LIST_NEXT(first_entry, entries)->data)) == other_number);

            elara_list_dealloc(subject, ^(void *entry){});
        });

        describe("with objects", ^{
            int number = 1;
            int other_number = 2;

            it("allows objects to be popped off the list easily", ^{
                subject = elara_list_create();
                expect(elara_list_pop(subject) == NULL);
                elara_list_insert(subject, (void *)&number);
                elara_list_insert(subject, (void *)&other_number);

                int *result = (int *)elara_list_pop(subject);
                expect(*result == other_number);
                expect(elara_list_count(subject) == 1);

                result = (int *)elara_list_pop(subject);
                expect(*result == number);
                expect(elara_list_count(subject) == 0);

                elara_list_dealloc(subject, ^(void *entry){});
            });

            it("allows the objects to be iterated over easily", ^{
                subject = elara_list_create();
                elara_list_insert(subject, (void *)&number);
                elara_list_insert(subject, (void *)&other_number);

                int expected_objects[] = {1, 2};
                int *expected_objects_hack = (int *)expected_objects;
                __block int index = 0;
                elara_list_foreach(subject, ^(void *entry){
                    int item = (*((int *)entry));
                    expect(expected_objects_hack[index] == item);
                    index += 1;
                });

                elara_list_dealloc(subject, ^(void *entry){});
            });

            it("allows any object in the list to be retrieved easily", ^{
                subject = elara_list_create();

                elara_list_insert(subject, (void *)&number);
                elara_list_insert(subject, (void *)&other_number);

                expect(*(int *)elara_list_get(subject, 0) == 1);
                expect(*(int *)elara_list_get(subject, 1) == 2);
                expect(elara_list_get(subject, 2) == NULL);

                elara_list_dealloc(subject, ^(void *entry){});
            });
        });
    });
}
