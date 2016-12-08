#include <stdio.h>
#include <sys/queue.h>

#include "Elara.h"
#include "ElaraList.h"

void ElaraListSpec() {
    describe("An ElaraList", ^{
        it("starts off with 0 objects", ^{
            ElaraList *subject = elara_list_create();

            expect(LIST_EMPTY(subject));
            expect(elara_list_count(subject) == 0);

            elara_list_dealloc(subject, ^(void *entry){});
        });

        it("allows an object to be appended easily", ^{
            ElaraList *subject = elara_list_create();

            int number = 1;
            elara_list_append(subject, &number);
            expect(elara_list_count(subject) == 1);
            expect(*((int *)(LIST_FIRST(subject)->data)) == number);

            int other_number = 2;
            elara_list_append(subject, &other_number);
            expect(elara_list_count(subject) == 2);
            ElaraListEntry *first_entry = LIST_FIRST(subject);
            expect(*((int *)(first_entry->data)) == number);
            expect(*((int *)(LIST_NEXT(first_entry, entries)->data)) == other_number);

            int third_number = 3;

            elara_list_insert(subject, &third_number, 1);
            expect(elara_list_count(subject) == 3);
            first_entry = LIST_FIRST(subject);

            expect(*((int *)(first_entry->data)) == number);
            ElaraListEntry *second_entry = LIST_NEXT(first_entry, entries);
            expect(*((int *)(second_entry->data)) == third_number);
            expect(*((int *)(LIST_NEXT(second_entry, entries)->data)) == other_number);

            int fourth_number = 4;

            elara_list_insert(subject, &fourth_number, -1);
            expect(elara_list_count(subject) == 4);

            first_entry = LIST_FIRST(subject);
            expect(*((int *)(first_entry->data)) == number);

            second_entry = LIST_NEXT(first_entry, entries);
            expect(*((int *)(second_entry->data)) == third_number);

            ElaraListEntry *third_entry = LIST_NEXT(second_entry, entries);
            expect(*((int *)(third_entry->data)) == other_number);

            expect(*((int *)(LIST_NEXT(third_entry, entries)->data)) == fourth_number);
            


            elara_list_dealloc(subject, ^(void *entry){});
        });

        describe("combining two lists", ^{
            __block ElaraList *first, *second;
            int one = 1;
            int two = 2;
            int three = 3;
            int four = 4;
            beforeEach(^{
                first = elara_list_create();
                second = elara_list_create();

                elara_list_append(first, (int *)&one);
                elara_list_append(first, (int *)&two);

                elara_list_append(second, (int *)&three);
                elara_list_append(second, (int *)&four);
            });

            afterEach(^{
                elara_list_dealloc(first, NULL);
                elara_list_dealloc(second, NULL);
            });

            it("can insert the elements of one list to any position of another", ^{
                elara_list_insert_list(first, second, 1);

                expect(elara_list_count(first) == 4);
                expect(elara_list_count(second) == 2);

                expect(*(int *)elara_list_get(first, 0) == 1);
                expect(*(int *)elara_list_get(first, 1) == 3);
                expect(*(int *)elara_list_get(first, 2) == 4);
                expect(*(int *)elara_list_get(first, 3) == 2);
            });

            it("will append the elements of a list to the first if given position -1", ^{
                elara_list_insert_list(first, second, -1);

                expect(elara_list_count(first) == 4);
                expect(elara_list_count(second) == 2);

                expect(*(int *)elara_list_get(first, 0) == 1);
                expect(*(int *)elara_list_get(first, 1) == 2);
                expect(*(int *)elara_list_get(first, 2) == 3);
                expect(*(int *)elara_list_get(first, 3) == 4);
            });
        });

        /*
        it("can combine two lists", ^{
            ElaraList *first = elara_list_create();
            ElaraList *second = elara_list_create();

            
        });
        */

        describe("with objects", ^{
            __block ElaraList *subject = NULL;

            int number = 1;
            int other_number = 2;

            beforeEach(^{
                subject = elara_list_create();
                elara_list_append(subject, (void *)&number);
                elara_list_append(subject, (void *)&other_number);
            });

            afterEach(^{
                elara_list_dealloc(subject, ^(void *entry){});
                subject = NULL;
            });

            it("allows objects to be popped off the list easily", ^{
                int *result = (int *)elara_list_pop(subject);
                expect(*result == other_number);
                expect(elara_list_count(subject) == 1);

                result = (int *)elara_list_pop(subject);
                expect(*result == number);
                expect(elara_list_count(subject) == 0);

                expect(elara_list_pop(subject) == NULL);
            });

            it("returns NULL if you try to grab an element outside the range of the list", ^{
                expect(elara_list_get(subject, 4) == NULL);
            });

            it("returns the last element if you try to grab the -1st element in the list", ^{
                expect(*(int *)elara_list_get(subject, -1) == other_number);
            });

            it("allows the objects to be iterated over easily", ^{
                int expected_objects[] = {1, 2};
                int *expected_objects_hack = (int *)expected_objects;
                __block int index = 0;
                elara_list_foreach(subject, ^(void *entry){
                    int item = (*((int *)entry));
                    expect(expected_objects_hack[index] == item);
                    index += 1;
                });
            });

            it("allows any object in the list to be retrieved easily", ^{
                expect(elara_list_count(subject) == 2);
                expect(*(int *)elara_list_get(subject, 0) == 1);
                expect(*(int *)elara_list_get(subject, 1) == 2);
                expect(elara_list_get(subject, 2) == NULL);
            });
        });
    });
}
