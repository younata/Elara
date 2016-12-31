#include <stdio.h>
#include <sys/queue.h>

#include "Elara.h"
#include "ElaraList.h"
#include "SpecHelper.h"

void ElaraListSpec() {
    describe("An ElaraList", ^{
        it("starts off with 0 objects", ^{
            ElaraList *subject = elara_list_create();

            int is_list_empty = LIST_EMPTY(subject);
            expect(&is_list_empty).to(be_truthy());
            int list_count = elara_list_count(subject);
            expect(&list_count).to(equal(0));
            expect(subject).to(have_list_size(0));

            elara_list_dealloc(subject, ^(void *entry){});
        });

        it("allows an object to be appended easily", ^{
            ElaraList *subject = elara_list_create();

            int number = 1;
            elara_list_append(subject, &number);
            expect(subject).to(have_list_size(1));
            expect(LIST_FIRST(subject)->data).to(equal(number));

            int other_number = 2;
            elara_list_append(subject, &other_number);
            expect(subject).to(have_list_size(2));
            ElaraListEntry *first_entry = LIST_FIRST(subject);
            expect(first_entry->data).to(equal(number));
            expect(LIST_NEXT(first_entry, entries)->data).to(equal(other_number));

            int third_number = 3;

            elara_list_insert(subject, &third_number, 1);
            expect(subject).to(have_list_size(3));
            first_entry = LIST_FIRST(subject);

            expect(first_entry->data).to(equal(number));
            ElaraListEntry *second_entry = LIST_NEXT(first_entry, entries);
            expect(second_entry->data).to(equal(third_number));
            expect(LIST_NEXT(second_entry, entries)->data).to(equal(other_number));

            int fourth_number = 4;

            elara_list_insert(subject, &fourth_number, -1);
            expect(subject).to(have_list_size(4));

            first_entry = LIST_FIRST(subject);
            expect(first_entry->data).to(equal(number));

            second_entry = LIST_NEXT(first_entry, entries);
            expect(second_entry->data).to(equal(third_number));

            ElaraListEntry *third_entry = LIST_NEXT(second_entry, entries);
            expect(third_entry->data).to(equal(other_number));

            expect(LIST_NEXT(third_entry, entries)->data).to(equal(fourth_number));

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

                expect(first).to(have_list_size(4));
                expect(second).to(have_list_size(2));

                expect(elara_list_get(first, 0)).to(equal(1));
                expect(elara_list_get(first, 1)).to(equal(3));
                expect(elara_list_get(first, 2)).to(equal(4));
                expect(elara_list_get(first, 3)).to(equal(2));
            });

            it("will append the elements of a list to the first if given position -1", ^{
                elara_list_insert_list(first, second, -1);

                expect(first).to(have_list_size(4));
                expect(second).to(have_list_size(2));

                expect(elara_list_get(first, 0)).to(equal(1));
                expect(elara_list_get(first, 1)).to(equal(2));
                expect(elara_list_get(first, 2)).to(equal(3));
                expect(elara_list_get(first, 3)).to(equal(4));
            });
        });

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
                expect(elara_list_pop(subject)).to(equal(other_number));
                expect(subject).to(have_list_size(1));

                expect(elara_list_pop(subject)).to(equal(number));
                expect(subject).to(have_list_size(0));

                expect(elara_list_pop(subject)).to(be_null());
            });

            it("returns NULL if you try to grab an element outside the range of the list", ^{
                expect(elara_list_get(subject, 4)).to(be_null());
            });

            it("returns the last element if you try to grab the -1st element in the list", ^{
                expect(elara_list_get(subject, -1)).to(equal(other_number));
            });

            it("allows the objects to be iterated over easily", ^{
                int expected_objects[] = {1, 2};
                int *expected_objects_hack = (int *)expected_objects;
                __block int index = 0;
                elara_list_foreach(subject, ^(void *entry){
                    int item = (*((int *)entry));
                    expect(&(expected_objects_hack[index])).to(equal(item));
                    index += 1;
                });
            });

            it("allows any object in the list to be retrieved easily", ^{
                expect(subject).to(have_list_size(2));
                expect(elara_list_get(subject, 0)).to(equal(1));
                expect(elara_list_get(subject, 1)).to(equal(2));
                expect(elara_list_get(subject, 2)).to(be_null());
            });
        });
    });
}
