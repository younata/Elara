#include <stdio.h>

#include "Elara.h"
#include "Stack.h"
#include "TestContext.h"

void TestContextSpec() {
    describe("A TestContext", ^{
        __block TestContext *subject;

        it("starts off with 0 children and empty everything", ^{
            subject = testContext_create(NULL);

            expect(subject->name == NULL);
            expect(subject->block == NULL);
            expect(subject->status == TestStatusNotATest);
            expect(elara_stack_count(subject->children) == 0);

            testContext_dealloc(subject);
        });

        it("can get inserted into a parent context if inited with that", ^{
            subject = testContext_create(NULL);

            TestContext *other = testContext_create(subject);
            expect(elara_stack_get(subject->children, 0) == other);

            testContext_dealloc(subject);
        });
    });
}
