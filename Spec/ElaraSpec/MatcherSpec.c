#include "Elara.h"
#include "Matchers.h"

#include <stdio.h>
#include <Block.h>
#include <stdlib.h>

void MatcherSpec() {
    describe("be_identical_to", ^{
        __block ElaraMatcherReturn matcher_return;

        afterEach(^{
            Block_release(matcher_return.evaluator);
            Block_release(matcher_return.failure_message_formatter);
        });

        describe("evaluator", ^{
            it("works when the value being tested is NULL", ^{
                int some_value = 1;
                matcher_return = be_identical_to(&some_value);

                elara_bool return_value = matcher_return.evaluator(NULL);

                expect(&return_value).to(equal(elara_false));
            });

            it("fails when the expected is NULL", ^{
                matcher_return = be_identical_to(NULL);

                elara_bool return_value = matcher_return.evaluator(NULL);

                expect(&return_value).to(equal(elara_false));
            });

            it("works when both values are non-null", ^{
                int some_value = 1;
                matcher_return = be_identical_to(&some_value);

                elara_bool return_value = matcher_return.evaluator(&some_value);

                expect(&return_value).to(equal(elara_true));
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
            });

            it("tells the user to use the be_null matcher when expected is NULL", ^{
                matcher_return = be_identical_to(NULL);

                int some_value = 1;
                char *return_value = matcher_return.failure_message_formatter(&some_value, "to");

                char *message = calloc(1024, 1);
                snprintf(message, 1024, "Error: Received expectation for NULL, did you mean to use be_null?");

                expect(return_value).to(equal_string(message));

                free(message);
            });

            it("works as expected when both values are non-null", ^{
                int some_value = 1;
                matcher_return = be_identical_to(&some_value);

                int other_value = 2;
                char *return_value = matcher_return.failure_message_formatter(&other_value, "to");

                char *message = calloc(1024, 1);
                snprintf(message, 1024, "Expected %p to be same address as %p", &other_value, &some_value);

                expect(return_value).to(equal_string(message));

                free(message);
            });
        });
    });
}
