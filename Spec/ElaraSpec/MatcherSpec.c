#include "Elara.h"
#include "Matchers.h"

#include <stdio.h>
#include <Block.h>
#include <stdlib.h>

void MatcherSpec() {
    describe("matchers", ^{
        __block ElaraMatcherReturn matcher_return;

        afterEach(^{
            Block_release(matcher_return.evaluator);
            Block_release(matcher_return.failure_message_formatter);
        });

        describe("be_identical_to", ^{
            describe("evaluator", ^{
                it("works when the value being tested is NULL", ^{
                    int some_value = 1;
                    matcher_return = be_identical_to(&some_value);

                    ElaraTestResult return_value = matcher_return.evaluator(NULL);

                    expect(&return_value).to(equal(ElaraTestResultFail));
                });

                it("fails when the expected is NULL", ^{
                    matcher_return = be_identical_to(NULL);

                    ElaraTestResult return_value = matcher_return.evaluator(NULL);

                    expect(&return_value).to(equal(ElaraTestResultError));
                });

                it("works when both values are non-null", ^{
                    int some_value = 1;
                    matcher_return = be_identical_to(&some_value);

                    ElaraTestResult return_value = matcher_return.evaluator(&some_value);

                    expect(&return_value).to(equal(ElaraTestResultPass));
                });
            });

            describe("failure_message_formatter", ^{
                it("works when the value being tested is NULL", ^{
                    int some_value = 1;
                    matcher_return = be_identical_to(&some_value);

                    char *return_value = matcher_return.failure_message_formatter(NULL, "to");

                    char *message = calloc(1024, 1);
                    snprintf(message, 1024, "Expected NULL to be same address as %p", &some_value);

                    expect(return_value).to(equal_string(message));

                    free(message);
                    free(return_value);
                });

                it("tells the user to use the be_null matcher when expected is NULL", ^{
                    matcher_return = be_identical_to(NULL);

                    int some_value = 1;
                    char *return_value = matcher_return.failure_message_formatter(&some_value, "to");

                    char *message = calloc(1024, 1);
                    snprintf(message, 1024, "Error: Received expectation for NULL, did you mean to use be_null?");

                    expect(return_value).to(equal_string(message));

                    free(message);
                    free(return_value);
                });

                it("works as expected when both values are non-null", ^{
                    int some_value = 1;
                    matcher_return = be_identical_to(&some_value);

                    int tested_value = 2;
                    char *return_value = matcher_return.failure_message_formatter(&tested_value, "to");

                    char *message = calloc(1024, 1);
                    snprintf(message, 1024, "Expected %p to be same address as %p", &tested_value, &some_value);

                    expect(return_value).to(equal_string(message));

                    free(message);
                    free(return_value);
                });
            });
        });

        describe("equal", ^{
            describe("evaluator", ^{
                it("correctly errors when the value being tested is NULL", ^{
                    matcher_return = equal(0);

                    ElaraTestResult return_value = matcher_return.evaluator(NULL);

                    expect(&return_value).to(equal(ElaraTestResultError));
                });

                it("can pass when the value being tested is an integer", ^{
                    matcher_return = equal(0);

                    int passing_value = 0;
                    int failing_value = 1;
                    ElaraTestResult return_value = matcher_return.evaluator(&passing_value);

                    expect(&return_value).to(equal(ElaraTestResultPass));

                    return_value = matcher_return.evaluator(&failing_value);
                    expect(&return_value).to(equal(ElaraTestResultFail));
                });
            });

            describe("failure_message_formatter", ^{
                it("describes the issue when NULL is passed as the tested value", ^{
                    matcher_return = equal(0);

                    char *return_value = matcher_return.failure_message_formatter(NULL, "to");

                    expect(return_value).to(equal_string("Received NULL, expected int *"));

                    free(return_value);
                });

                it("describes the issue for other values", ^{
                    matcher_return = equal(0);

                    int some_value = 1;
                    char *return_value = matcher_return.failure_message_formatter(&some_value, "to");

                    expect(return_value).to(equal_string("expected 1 to equal 0"));

                    free(return_value);
                });
            });
        });

        describe("almost_equal", ^{
            describe("evaluator", ^{
                it("correctly errors when the value being tested is NULL", ^{
                    matcher_return = almost_equal(0, 3);

                    ElaraTestResult return_value = matcher_return.evaluator(NULL);

                    expect(&return_value).to(equal(ElaraTestResultError));
                });

                it("can pass when the value being tested can be coerced into a double", ^{
                    matcher_return = almost_equal(0, 3);

                    double passing_value = 0;
                    double failing_value = 0.005;
                    ElaraTestResult return_value = matcher_return.evaluator(&passing_value);

                    expect(&return_value).to(equal(ElaraTestResultPass));

                    return_value = matcher_return.evaluator(&failing_value);
                    expect(&return_value).to(equal(ElaraTestResultFail));
                });
            });

            describe("failure_message_formatter", ^{
                it("describes the issue when NULL is passed in as the tested value", ^{
                    matcher_return = almost_equal(0, 0);

                    char *return_value = matcher_return.failure_message_formatter(NULL, "to");

                    expect(return_value).to(equal_string("Received NULL, expected double *"));

                    free(return_value);
                });

                it("describes the issue for other values", ^{
                    matcher_return = almost_equal(0, 3);

                    double some_value = 1;
                    char *return_value = matcher_return.failure_message_formatter(&some_value, "to");

                    expect(return_value).to(equal_string("Expected 1.000000 to equal 0.000000 (within 3 decimal places)"));

                    free(return_value);
                });
            });
        });

        xdescribe("equal_string", ^{

        });

        xdescribe("be_null", ^{

        });

        xdescribe("be_truthy", ^{
            
        });
    });
}
