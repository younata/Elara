#include <stdio.h>
#include <sys/queue.h>

#include "Elara.h"
#include "Stack.h"

void StackSpec() {
    describe("An ElaraStack", ^{
        __block ElaraStack *subject;

        it("starts off with 0 objects", ^{
            subject = elara_stack_create();

            expect(SLIST_EMPTY(subject));
            expect(elara_stack_count(subject) == 0);

            elara_stack_dealloc(subject, ^(void *entry){});
        });

        it("allows an object to be inserted easily", ^{
            subject = elara_stack_create();

            int number = 1;
            elara_stack_insert(subject, &number);
            expect(elara_stack_count(subject) == 1);
            expect(*((int *)(SLIST_FIRST(subject)->data)) == number);

            int other_number = 2;
            elara_stack_insert(subject, &other_number);
            expect(elara_stack_count(subject) == 2);
            expect(*((int *)(SLIST_FIRST(subject)->data)) == other_number);

            elara_stack_dealloc(subject, ^(void *entry){});
        });

        describe("with objects", ^{
            int number = 1;
            int other_number = 2;

            it("allows objects to be popped off the list easily", ^{
                subject = elara_stack_create();
                elara_stack_insert(subject, (void *)&number);
                elara_stack_insert(subject, (void *)&other_number);

                int *result = (int *)elara_stack_pop(subject);
                expect(*result == other_number);
                expect(elara_stack_count(subject) == 1);

                result = (int *)elara_stack_pop(subject);
                expect(*result == number);
                expect(elara_stack_count(subject) == 0);

                elara_stack_dealloc(subject, ^(void *entry){});
            });

            it("allows the objects to be iterated over easily", ^{
                subject = elara_stack_create();
                elara_stack_insert(subject, (void *)&number);
                elara_stack_insert(subject, (void *)&other_number);

                __block int received_count = 0;
                elara_stack_foreach(subject, ^(void *entry){
                    received_count += (*((int *)entry));
                });
                expect(received_count == 3);

                elara_stack_dealloc(subject, ^(void *entry){});
            });

            it("allows any object in the list to be retrieved easily", ^{
                subject = elara_stack_create();

                elara_stack_insert(subject, (void *)&number);
                elara_stack_insert(subject, (void *)&other_number);

                expect(*(int *)elara_stack_get(subject, 0) == 1);
                expect(*(int *)elara_stack_get(subject, 1) == 2);
                expect(elara_stack_get(subject, 2) == NULL);

                elara_stack_dealloc(subject, ^(void *entry){});
            });
        });
    });
}
