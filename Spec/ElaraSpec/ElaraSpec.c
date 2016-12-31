#include <stdio.h>

#include "Elara.h"

void ElaraSpec() {
    __block int runs_run_count = 0;
    __block int supports_multiple_it_blocks_run_count = 0;
    __block int nested_tests_run_count = 0;
    describe("elara specs", ^{
        it("runs", ^{
            runs_run_count += 1;
            expect(&runs_run_count).to(equal(1));
        });

        it("supports multiple it blocks", ^{
            supports_multiple_it_blocks_run_count += 1;
            expect(&supports_multiple_it_blocks_run_count).to(equal(1));
        });

        xit("skips xit'd tests", ^{
            expect(NULL).to(equal(1));
        });

        it("skips tests with NULL test blocks", NULL);

        describe("nested tests", ^{
            it("are also supported", ^{
                nested_tests_run_count += 1;
                expect(&nested_tests_run_count).to(equal(1));
            });
        });
    });

    xdescribe("xdescribe blocks", ^{
        it("are skipped", ^{
            expect(NULL).to(equal(1));
        });

        fit("even with focused children", ^{
            expect(NULL).to(equal(1));
        });
    });
}
