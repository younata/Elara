#include "Elara.h"
#include "ElaraList.h"

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
            int list_count = elara_list_count(list);
            expect(&list_count).to(equal(2));
            expect(elara_list_get(list, 0)).to(equal(outerOne));
            expect(elara_list_get(list, 1)).to(equal(outerTwo));
        });

        it("executes the beforeEaches a second time, prior to this clojure", ^{
            int list_count = elara_list_count(list);
            expect(&list_count).to(equal(4));
            expect(elara_list_get(list, 0)).to(equal(outerOne));
            expect(elara_list_get(list, 1)).to(equal(outerTwo));
            expect(elara_list_get(list, 2)).to(equal(outerOne));
        });

        describe("when there are nested beforeEaches", ^{
            beforeEach(^{ elara_list_append(list, &innerOne); });
            beforeEach(^{ elara_list_append(list, &innerTwo); });

            it("executes the nested beforeEaches after the previous beforeEaches", ^{
                int list_count = elara_list_count(list);
                expect(&list_count).to(equal(8));
                expect(elara_list_get(list, 0)).to(equal(outerOne));
                expect(elara_list_get(list, 1)).to(equal(outerTwo));
                expect(elara_list_get(list, 2)).to(equal(outerOne));
                expect(elara_list_get(list, 3)).to(equal(outerTwo));

                expect(elara_list_get(list, 4)).to(equal(outerOne));
                expect(elara_list_get(list, 5)).to(equal(outerTwo));
                expect(elara_list_get(list, 6)).to(equal(innerOne));
                expect(elara_list_get(list, 7)).to(equal(innerTwo));
            });
        });

        describe("when there are nested beforeEaches without examples", ^{
            beforeEach(^{ elara_list_append(list, &noExamples); });
        });
    });
}
