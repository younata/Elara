#include "Elara.h"
#include "List.h"

void BeforeEachSpec() {
    ElaraList *list = elara_list_create();
    __block int outerOne = 1;
    __block int outerTwo = 2;
    __block int innerOne = 4;
    __block int innerTwo = 8;
    __block int noExamples = 16;

    describe("Before Eaches", ^{
        beforeEach(^{ elara_list_append(list, &outerOne); });
        beforeEach(^{ elara_list_append(list, &outerTwo); });

        it("executes the beforeEaches once, prior to this clojure", ^{
            expect(elara_list_count(list) == 2);
            expect(*(int *)elara_list_get(list, 0) == outerOne);
            expect(*(int *)elara_list_get(list, 1) == outerTwo);
        });

        it("executes the beforeEaches a second time, prior to this clojure", ^{
            expect(elara_list_count(list) == 4);
            expect(*(int *)elara_list_get(list, 0) == outerOne);
            expect(*(int *)elara_list_get(list, 1) == outerTwo);
            expect(*(int *)elara_list_get(list, 2) == outerOne);
        });

        describe("when there are nested beforeEaches", ^{
            beforeEach(^{ elara_list_append(list, &innerOne); });
            beforeEach(^{ elara_list_append(list, &innerTwo); });

            it("executes the nested beforeEaches after the previous beforeEaches", ^{
                expect(elara_list_count(list) == 8);
                expect(*(int *)elara_list_get(list, 0) == outerOne);
                expect(*(int *)elara_list_get(list, 1) == outerTwo);
                expect(*(int *)elara_list_get(list, 2) == outerOne);
                expect(*(int *)elara_list_get(list, 3) == outerTwo);

                expect(*(int *)elara_list_get(list, 4) == outerOne);
                expect(*(int *)elara_list_get(list, 5) == outerTwo);
                expect(*(int *)elara_list_get(list, 6) == innerOne);
                expect(*(int *)elara_list_get(list, 7) == innerTwo);
            });
        });

        describe("when there are nested beforeEaches without examples", ^{
            beforeEach(^{ elara_list_append(list, &noExamples); });
        });
    });
}
