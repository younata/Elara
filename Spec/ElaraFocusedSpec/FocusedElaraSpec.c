#include "Elara.h"

void ElaraSpec() {
    __block int unfocused_run_count = 0;
    __block int focused_run_count = 0;
    __block int supports_multiple_it_blocks_run_count = 0;
    __block int unfocused_nested_tests_run_count = 0;
    __block int focused_nested_tests_run_count = 0;
    __block int focused_describe_tests_run_count = 0;

    describe("focused elara specs", ^{
        it("does not run", ^{
            unfocused_run_count = 1;
            expect(&unfocused_run_count).toNot(equal(1));
        });

        fit("only runs focused specs", ^{
            focused_run_count += 1;
            expect(&focused_run_count).to(equal(1));
        });

        describe("nested focused tests", ^{
            it("are also followed through with (but not this one)", ^{
                unfocused_nested_tests_run_count = 1;
                expect(&unfocused_nested_tests_run_count).toNot(equal(1));
            });

            fit("are also followed through with", ^{
                focused_nested_tests_run_count += 1;
                expect(&focused_nested_tests_run_count).to(equal(1));
            });
        });

        fdescribe("nested focused tests", ^{
            it("are also followed through with", ^{
                focused_describe_tests_run_count += 1;
                expect(&focused_describe_tests_run_count).to(equal(1));
            });
        });
    });
}

void OtherElaraBlock() {
    __block int focused_describe_tests_run_count = 0;

    fdescribe("top-level focused specs", ^{
        describe("nested focused tests", ^{
            describe("with an additional level of nesting", ^{
                it("are also followed through with", ^{
                    focused_describe_tests_run_count += 1;
                    expect(&focused_describe_tests_run_count).to(equal(1));
                });
            });
        });
    });

    describe("other top level tests", ^{
        it("does not run", ^{
            int this = 1;
            int to_not_run = 0;
            expect(&this).to(equal(to_not_run));
        });
    });
}
