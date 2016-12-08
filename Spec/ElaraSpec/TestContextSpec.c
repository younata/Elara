#include <stdio.h>

#include "Elara.h"
#include "ElaraList.h"
#include "TestContext.h"
#include "string.h"

void TestContextSpec() {
    describe("A TestContext", ^{
        __block TestContext *subject;

        it("starts off with 0 children and empty everything", ^{
            subject = testContext_create(NULL, TestFocusUnfocused);

            expect(subject->name == NULL);
            expect(subject->block == NULL);
            expect(subject->status == TestStatusNotATest);
            expect(subject->children != NULL);
            expect(elara_list_count(subject->children) == 0);

            testContext_dealloc(subject);
        });

        it("can get inserted into a parent context if inited with that", ^{
            subject = testContext_create(NULL, TestFocusUnfocused);

            TestContext *other = testContext_create(subject, TestFocusUnfocused);
            expect(elara_list_get(subject->children, 0) == other);

            testContext_dealloc(subject);
        });

        it("causes children to inherit parent's focusing", ^{
            TestContext *parent = testContext_create(NULL, TestFocusFocused);

            TestContext *child = testContext_create(parent, TestFocusUnfocused);

            expect(child->focus == TestFocusFocused);

            testContext_dealloc(parent);
        });

        it("causes children to inherit parent's skipped", ^{
            TestContext *parent = testContext_create(NULL, TestFocusSkipped);

            TestContext *child = testContext_create(parent, TestFocusUnfocused);

            expect(child->focus == TestFocusSkipped);

            testContext_dealloc(parent);
        });

        it("skipped children do not inherit parent's focus", ^{
            TestContext *parent = testContext_create(NULL, TestFocusFocused);

            TestContext *child = testContext_create(parent, TestFocusSkipped);

            expect(child->focus == TestFocusSkipped);

            testContext_dealloc(parent);
        });

        it("correctly calculates it's full name", ^{
            TestContext *parent = testContext_create(NULL, TestFocusUnfocused);

            TestContext *child1 = testContext_create(parent, TestFocusUnfocused);
            child1->name = "child1";
            TestContext *child2 = testContext_create(child1, TestFocusUnfocused);
            child2->name = "child2";
            TestContext *child3 = testContext_create(child2, TestFocusUnfocused);
            child3->name = "child3";

            expect(strncmp(testContext_full_test_name(child1), "child1", 7) == 0);
            expect(strncmp(testContext_full_test_name(child2), "child1 child2", 14) == 0);
            expect(strncmp(testContext_full_test_name(child3), "child1 child2 child3", 21) == 0);

            testContext_dealloc(parent);
        });
    });
}
