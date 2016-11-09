#include "Elara.h"
#include "List.h"

void AfterEachSpec() {
    ElaraList *list = elara_list_create();
    __block int outerOne = 1;
    __block int outerTwo = 2;
    __block int innerOne = 4;
    __block int innerTwo = 8;
    __block int noExamples = 16;

    describe("After Eaches", ^{
        afterEach(^{ elara_list_append(list, &outerOne); });
        afterEach(^{ elara_list_append(list, &outerTwo); });

        it("executes the afterEaches once, after this clojure", ^{
            expect(elara_list_count(list) == 0);
        });

        it("executes the afterEaches a second time, after this clojure", ^{
            expect(elara_list_count(list) == 2);
            expect(*(int *)elara_list_get(list, 0) == outerOne);
            expect(*(int *)elara_list_get(list, 1) == outerTwo);
        });

        describe("when there are nested afterEaches", ^{
            afterEach(^{ elara_list_append(list, &innerOne); });
            afterEach(^{ elara_list_append(list, &innerTwo); });

            it("executes the nested afterEaches before the previous afterEaches", ^{
                expect(elara_list_count(list) == 4);
                expect(*(int *)elara_list_get(list, 0) == outerOne);
                expect(*(int *)elara_list_get(list, 1) == outerTwo);
                expect(*(int *)elara_list_get(list, 2) == outerOne);
                expect(*(int *)elara_list_get(list, 3) == outerTwo);
            });

            it("executes the nested afterEaches before the previous afterEaches", ^{
                expect(elara_list_count(list) == 8);
                expect(*(int *)elara_list_get(list, 0) == outerOne);
                expect(*(int *)elara_list_get(list, 1) == outerTwo);
                expect(*(int *)elara_list_get(list, 2) == outerOne);
                expect(*(int *)elara_list_get(list, 3) == outerTwo);

                expect(*(int *)elara_list_get(list, 4) == innerOne);
                expect(*(int *)elara_list_get(list, 5) == innerTwo);
                expect(*(int *)elara_list_get(list, 6) == outerOne);
                expect(*(int *)elara_list_get(list, 7) == outerTwo);
            });
        });

        describe("when there are nested afterEaches without examples", ^{
            afterEach(^{ elara_list_append(list, &noExamples); });
        });
    });
}
