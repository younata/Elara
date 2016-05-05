#include "Elara.h"

elara_tests(^{
    describe("a test", ^{
        it("runs", ^{
            expect(1 == 1);
        });

        it("supports multiple it blocks", ^{
            expect(1);
        });

        describe("nested tests", ^{
            it("are also supported", ^{
                expect(1);
            });
        });
    });
});

