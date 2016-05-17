#include "Elara.h"
#include <stdio.h>

typedef struct {
    int outerOne;
    int outerTwo;
    int innerOne;
    int innerTwo;
    int noExamples;
} AfterEachCalls;

void AfterEachSpec() {
    __block AfterEachCalls afterEachCalls = {
        .outerOne = 0,
        .outerTwo = 0,
        .innerOne = 0,
        .innerTwo = 0,
        .noExamples = 0
    };

    describe("After Eaches", ^{
        afterEach(^{ afterEachCalls.outerOne += 1; });
        afterEach(^{ afterEachCalls.outerTwo += 1; });

        it("executes the afterEaches once, after this clojure", ^{
            expect(afterEachCalls.outerOne == 0);
            expect(afterEachCalls.outerTwo == 0);
            expect(afterEachCalls.innerOne == 0);
            expect(afterEachCalls.innerTwo == 0);
            expect(afterEachCalls.noExamples == 0);
        });

        it("executes the afterEaches a second time, after this clojure", ^{
            expect(afterEachCalls.outerOne == 1);
            expect(afterEachCalls.outerTwo == 1);
            expect(afterEachCalls.innerOne == 0);
            expect(afterEachCalls.innerTwo == 0);
            expect(afterEachCalls.noExamples == 0);
        });

        describe("when there are nested afterEaches", ^{
            afterEach(^{ afterEachCalls.innerOne += 1; });
            afterEach(^{ afterEachCalls.innerTwo += 1; });

            it("executes the nested afterEaches after the previous afterEaches", ^{
                expect(afterEachCalls.outerOne == 2);
                expect(afterEachCalls.outerTwo == 2);
                expect(afterEachCalls.innerOne == 0);
                expect(afterEachCalls.innerTwo == 0);
                expect(afterEachCalls.noExamples == 0);
            });

            it("executes the nested afterEaches after the previous afterEaches", ^{
                expect(afterEachCalls.outerOne == 3);
                expect(afterEachCalls.outerTwo == 3);
                expect(afterEachCalls.innerOne == 1);
                expect(afterEachCalls.innerTwo == 1);
                expect(afterEachCalls.noExamples == 0);
            });
        });

        describe("when there are nested afterEaches without examples", ^{
            afterEach(^{ afterEachCalls.noExamples += 1; });
        });
    });
}
