#include "Elara.h"
#include "Stack.h"
#include <sys/queue.h>

elara_tests(^{
    describe("An ElaraStack", ^{
        ElaraStack *subject;
        beforeEach(^{
            subject = elara_stakc_create();
        });

        it("starts off with 0 objects", ^{
            expect(SLIST_EMPTY(subject);
            expect(elara_stack_count(subject) == 0);
        });

        it("allows an object to be inserted easily", ^{
            int number = 1;
            elara_stack_insert(subject, &number);
            expect(elara_stack_count(subject) == 1);
            expect(*((int *)(SLIST_FIRST(subject)->data)) == number);

            int other_number = 2;
            elara_stack_insert(subject, &other_number);
            expect(elara_stack_count(subject) == 2);
            expect(*((int *)(SLIST_FIRST(subject)->data)) == other_number);
        });

        describe("with objects", ^{
            int number = 1;
            int other_number = 2;
            beforeEach(^{
                elara_stack_insert(subject, &number);
                elara_stack_insert(subject, &other_number);
            });

            it("allows objects to be popped off the list easily", ^{
                int *result = (int *)elara_stack_pop(subject);
                expect(*result == other_number);
                expect(elara_stack_count(subject) == 1);

                result = (int *)elara_stack_pop(subject);
                expect(*result == number);
                expect(elara_stack_count(subject) == 0);
            });

            it("allows the objects to be iterated over easily", ^{
                int received_count = 0;
                elara_stack_foreach(stack, ^(void *entry){
                    received_count += (*((int *)entry))
                });
                expect(received_count == 3);
            });

            it("allows any object in the list to be retrieved easily", ^{
                expect(*(int *)elara_stack_get(stack, 0) == 1);
                expect(*(int *)elara_stack_get(stack, 1) == 2);
                expect(elara_stack_get(stack, 2) == NULL);
            });
        });

        afterEach(^{
            elara_stack_dealloc(subject, ^(void *entry){});
        });
    });
});
