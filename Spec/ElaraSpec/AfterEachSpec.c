#include "Elara.h"
#include "ElaraList.h"

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
            int list_count = elara_list_count(list);
            expect(&list_count).to(equal(0));
        });

        it("executes the afterEaches a second time, after this clojure", ^{
            int list_count = elara_list_count(list);
            expect(&list_count).to(equal(2));
            expect(elara_list_get(list, 0)).to(equal(outerOne));
            expect(elara_list_get(list, 1)).to(equal(outerTwo));
        });

        describe("when there are nested afterEaches", ^{
            afterEach(^{ elara_list_append(list, &innerOne); });
            afterEach(^{ elara_list_append(list, &innerTwo); });

            it("executes the nested afterEaches before the previous afterEaches", ^{
                int list_count = elara_list_count(list);
                expect(&list_count).to(equal(4));
                expect(elara_list_get(list, 0)).to(equal(outerOne));
                expect(elara_list_get(list, 1)).to(equal(outerTwo));
                expect(elara_list_get(list, 2)).to(equal(outerOne));
                expect(elara_list_get(list, 3)).to(equal(outerTwo));
            });

            it("executes the nested afterEaches before the previous afterEaches", ^{
                int list_count = elara_list_count(list);
                expect(&list_count).to(equal(8));
                expect(elara_list_get(list, 0)).to(equal(outerOne));
                expect(elara_list_get(list, 1)).to(equal(outerTwo));
                expect(elara_list_get(list, 2)).to(equal(outerOne));
                expect(elara_list_get(list, 3)).to(equal(outerTwo));

                expect(elara_list_get(list, 4)).to(equal(innerOne));
                expect(elara_list_get(list, 5)).to(equal(innerTwo));
                expect(elara_list_get(list, 6)).to(equal(outerOne));
                expect(elara_list_get(list, 7)).to(equal(outerTwo));
            });
        });

        describe("when there are nested afterEaches without examples", ^{
            afterEach(^{ elara_list_append(list, &noExamples); });
        });
    });
}
