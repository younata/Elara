#include "Elara.h"

typedef struct {
    int outerOne;
    int outerTwo;
    int innerOne;
    int innerTwo;
    int noExamples;
} BeforeEachCalls;

void BeforeEachSpec() {
    __block BeforeEachCalls beforeEachCalls = {
        .outerOne = 0,
        .outerTwo = 0,
        .innerOne = 0,
        .innerTwo = 0,
        .noExamples = 0
    };

    describe("Before Eaches", ^{
        beforeEach(^{ beforeEachCalls.outerOne += 1; });
        beforeEach(^{ beforeEachCalls.outerTwo += 1; });

        it("executes the beforeEaches once, prior to this clojure", ^{
            expect(beforeEachCalls.outerOne == 1);
            expect(beforeEachCalls.outerTwo == 1);
            expect(beforeEachCalls.innerOne == 0);
            expect(beforeEachCalls.innerTwo == 0);
            expect(beforeEachCalls.noExamples == 0);
        });

        it("executes the beforeEaches a second time, prior to this clojure", ^{
            expect(beforeEachCalls.outerOne == 2);
            expect(beforeEachCalls.outerTwo == 2);
            expect(beforeEachCalls.innerOne == 0);
            expect(beforeEachCalls.innerTwo == 0);
            expect(beforeEachCalls.noExamples == 0);
        });

        describe("when there are nested beforeEaches", ^{
            beforeEach(^{ beforeEachCalls.innerOne += 1; });
            beforeEach(^{ beforeEachCalls.innerTwo += 1; });

            it("executes the nested beforeEaches after the previous beforeEaches", ^{
                expect(beforeEachCalls.outerOne == 3);
                expect(beforeEachCalls.outerTwo == 3);
                expect(beforeEachCalls.innerOne == 1);
                expect(beforeEachCalls.innerTwo == 1);
                expect(beforeEachCalls.noExamples == 0);
            });
        });

        describe("when there are nested beforeEaches without examples", ^{
            beforeEach(^{ beforeEachCalls.noExamples += 1; });
        });
    });
}
