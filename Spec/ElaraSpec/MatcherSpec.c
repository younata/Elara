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

        describe("equal_string", ^{
            describe("evaluator", ^{
                it("correctly errors when the value being tested is NULL", ^{
                    matcher_return = equal_string("string");

                    ElaraTestResult return_value = matcher_return.evaluator(NULL);

                    expect(&return_value).to(equal(ElaraTestResultError));
                });

                it("can pass when the value being tested is an actual string", ^{
                    matcher_return = equal_string("string");

                    ElaraTestResult return_value = matcher_return.evaluator("string");

                    expect(&return_value).to(equal(ElaraTestResultPass));

                    return_value = matcher_return.evaluator("nope");

                    expect(&return_value).to(equal(ElaraTestResultFail));
                });

                it("doesn't pass when substrings are passed in", ^{
                    matcher_return = equal_string("string");

                    ElaraTestResult return_value = matcher_return.evaluator("str");

                    expect(&return_value).to(equal(ElaraTestResultFail));
                });

                it("doesn't pass when superstrings are passed in", ^{
                    matcher_return = equal_string("string");

                    ElaraTestResult return_value = matcher_return.evaluator("stringstring");

                    expect(&return_value).to(equal(ElaraTestResultFail));
                });
            });

            describe("failure_message_formatter", ^{
                it("describes the issue when NULL is passed in as the tested value", ^{
                    matcher_return = equal_string("string");

                    char *return_value = matcher_return.failure_message_formatter(NULL, "to");

                    expect(return_value).to(equal_string("Received NULL, expected char *"));

                    free(return_value);
                });

                it("describes the issue for other values", ^{
                    matcher_return = equal_string("expected");

                    char *return_value = matcher_return.failure_message_formatter("tested", "to");

                    expect(return_value).to(equal_string("Expected 'tested' to equal 'expected'"));

                    free(return_value);
                });
            });
        });

        describe("be_null", ^{
            describe("evaluator", ^{
                it("passes only when the value being tested is NULL", ^{
                    matcher_return = be_null();

                    ElaraTestResult return_value = matcher_return.evaluator("stringstring");

                    expect(&return_value).to(equal(ElaraTestResultFail));

                    return_value = matcher_return.evaluator(NULL);

                    expect(&return_value).to(equal(ElaraTestResultPass));
                });
            });

            describe("failure_message_formatter", ^{
                it("says that it expected something to be null", ^{
                    matcher_return = be_null();

                    char *foo = "foo";
                    char *return_value = matcher_return.failure_message_formatter(foo, "to");

                    char *expected_message = calloc(1024, 1);
                    snprintf(expected_message, 1024, "Expected %p to be null", foo);
                    expect(return_value).to(equal_string(expected_message));

                    free(return_value);
                    free(expected_message);
                });

                it("still handles the case when tested value is null", ^{
                    matcher_return = be_null();

                    char *return_value = matcher_return.failure_message_formatter(NULL, "to");

                    expect(return_value).to(equal_string("Expected NULL to be null"));

                    free(return_value);
                });
            });
        });

        describe("be_truthy", ^{
            describe("evaluator", ^{
                it("correctly errors when NULL is passed in", ^{
                    matcher_return = be_truthy();

                    ElaraTestResult result = matcher_return.evaluator(NULL);

                    expect(&result).to(equal(ElaraTestResultError));
                });

                it("passes when a pointer to a non-zero value is passed in", ^{
                    matcher_return = be_truthy();

                    double value = 0.1;
                    ElaraTestResult result = matcher_return.evaluator(&value);

                    expect(&result).to(equal(ElaraTestResultPass));
                });

                it("fails when a pointer to a zero is passed in", ^{
                    matcher_return = be_truthy();

                    double value = 0;
                    ElaraTestResult result = matcher_return.evaluator(&value);

                    expect(&result).to(equal(ElaraTestResultFail));
                });
            });

            describe("failure_message_formatter", ^{
                it("describes the issue when NULL is passed in as the tested value", ^{
                    matcher_return = be_truthy();

                    char *return_value = matcher_return.failure_message_formatter(NULL, "to");

                    expect(return_value).to(equal_string("Expected NULL to be truthy"));

                    free(return_value);
                });

                it("describes the issue for other values", ^{
                    matcher_return = be_truthy();

                    double value = 0;
                    char *return_value = matcher_return.failure_message_formatter(&value, "to");

                    char *message = calloc(1024, 1);
                    snprintf(message, 1024, "Expected %p to be truthy", &value);
                    expect(return_value).to(equal_string(message));

                    free(return_value);
                    free(message);
                });
            });
        });
    });
}
