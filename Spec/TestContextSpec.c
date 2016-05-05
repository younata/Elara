#include "Elara.h"
#include "Stack.h"
#include "TestContext.h"

elara_tests(^{
    describe("A TestContext", ^{
        TestContext *subject;
        beforeEach(^{
            subject = testContext_create(NULL);
        });

        it("starts off with 0 children and empty everything", ^{
            expect(subject->name == NULL);
            expect(subject->block == NULL);
            expect(subject->status == TestStatusNotATest);
            expect(elara_stack_count(subject->children) == 0);
        });

        it("can get inserted into a parent context if inited with that") {
            TestContext *other = testContext_create(subject);
            expect(elara_stack_get(subject->children, 0) == other);
        }

        afterEach(^{
            testContext_dealloc(subject);
        });
    });
});
