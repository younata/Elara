#include "Elara.h"

void ElaraSpec() {
    __block int unfocused_run_count = 0;
    __block int focused_run_count = 0;
    __block int supports_multiple_it_blocks_run_count = 0;
    __block int unfocused_nested_tests_run_count = 0;
    __block int focused_nested_tests_run_count = 0;
    __block int focused_describe_tests_run_count = 0;

    describe("elara specs", ^{
        it("does not run", ^{
            unfocused_run_count = 1;
            expect(unfocused_run_count != 1);
        });

        fit("only runs focused specs", ^{
            focused_run_count += 1;
            expect(focused_run_count == 1);
        });

        it("supports multiple it blocks", ^{
            supports_multiple_it_blocks_run_count += 1;
            expect(supports_multiple_it_blocks_run_count == 1);
        });

        describe("nested focused tests", ^{
            it("are also followed through with", ^{
                unfocused_nested_tests_run_count = 1;
                expect(unfocused_nested_tests_run_count == 1);
            });

            fit("are also followed through with", ^{
                focused_nested_tests_run_count += 1;
                expect(focused_nested_tests_run_count == 1);
            });
        });

        fdescribe("nested focused tests", ^{
            it("are also followed through with", ^{
                focused_describe_tests_run_count += 1;
                expect(focused_describe_tests_run_count == 1);
            });
        });
    });
}