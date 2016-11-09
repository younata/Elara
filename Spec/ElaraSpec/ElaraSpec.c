#include "Elara.h"

void ElaraSpec() {
    __block int runs_run_count = 0;
    __block int supports_multiple_it_blocks_run_count = 0;
    __block int nested_tests_run_count = 0;
    describe("elara specs", ^{
        it("runs", ^{
            runs_run_count += 1;
            expect(runs_run_count == 1);
        });

        it("supports multiple it blocks", ^{
            supports_multiple_it_blocks_run_count += 1;
            expect(supports_multiple_it_blocks_run_count == 1);
        });

        describe("nested tests", ^{
            it("are also supported", ^{
                nested_tests_run_count += 1;
                expect(nested_tests_run_count == 1);
            });
        });
    });
}
