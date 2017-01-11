#include "Matchers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <Block.h>

#pragma mark - Environment

void (*ElaraEnvironmentAssert)(ElaraTestResult, char *, const char *, int);

void setElaraEnvironmentAssert(void (*elara_assert)(ElaraTestResult, char *, const char *, int)) {
    ElaraEnvironmentAssert = elara_assert;
}

#pragma mark - Matchers

ElaraMatcherReturn matcher_create(ElaraMatcherEvaluator evaluator, ElaraMatcherFormatter formatter) {
    ElaraMatcherReturn value;

    value.evaluator = Block_copy(evaluator);
    value.failure_message_formatter = Block_copy(formatter);
    return value;
}

void matcher_dealloc(ElaraMatcherReturn matcher_return) {
    Block_release(matcher_return.evaluator);
    Block_release(matcher_return.failure_message_formatter);
}

ElaraMatcherReturn be_identical_to(void *expected) {
    return matcher_create(^ElaraTestResult(void *received) {
        if (expected == NULL) {
            return ElaraTestResultError;
        }
        if (expected == received) {
            return ElaraTestResultPass;
        }
        return ElaraTestResultFail;
    },
    ^char *(void *received, char *to) {
        size_t message_length = 70 + strlen(to);
        char *message = calloc(message_length, 1);
        if (expected == NULL) {
            strncpy(message, "Error: Received expectation for NULL, did you mean to use be_null?", message_length);
        } else {
            if (received == NULL) {
                snprintf(message, message_length, "Expected NULL %s be same address as %p", to, expected);
            } else {
                snprintf(message, message_length, "Expected %p %s be same address as %p", received, to, expected);
            }
        }
        return message;
    });
}

ElaraMatcherReturn equal(int expected) {
    size_t (^decimal_places_in_int)(int) = ^size_t(int value) {
        if (value == 0) {
            return 1;
        }
        size_t decimal_places = (size_t)ceil(log10(value));
        if (value < 0) {
            decimal_places += 1;
        }
        return decimal_places;
    };

    return matcher_create(^ElaraTestResult(void *received) {
        if (received == NULL) {
            return ElaraTestResultError;
        }
        int value = *(int *)received;
        return value == expected;
    },
    ^char *(void *received, char *to) {
        if (received == NULL) {
            char *message = calloc(30, 1);
            snprintf(message, 30, "Received NULL, expected int *");
            return message;
        }
        int value = *(int *)received;

        size_t expected_message_size = strlen(to) + decimal_places_in_int(value) +
            decimal_places_in_int(expected) + 20;
        char *message = calloc(expected_message_size, 1);
        snprintf(message, expected_message_size, "expected %d %s equal %d", value, to, expected);

        return message;
    });
}

ElaraMatcherReturn almost_equal(double expected, double decimal_places) {
    return matcher_create(^ElaraTestResult(void *received) {
        if (received == NULL) {
            return ElaraTestResultError;
        }
        double value = *(double *)received;

        if (fabs(value - expected) < pow(10, -decimal_places)) {
            return ElaraTestResultPass;
        } else {
            return ElaraTestResultFail;
        }
    },
    ^char *(void *received, char *to) {
        if (received == NULL) {
            char *message = calloc(33, 1);
            snprintf(message, 33, "Received NULL, expected double *");
            return message;
        }
        double value = *(double *)received;

        char *message = calloc(1024, 1);
        snprintf(message, 1024, "Expected %f %s equal %f (within %d decimal places)", value, to, expected, (int)decimal_places);
        return message;
    });
}

ElaraMatcherReturn equal_string(char *expected) {
    size_t expected_length = strlen(expected);

    return matcher_create(^ElaraTestResult(void *received) {
        if (received == NULL) {
            return ElaraTestResultError;
        }
        char *received_as_string = (char *)received;

        size_t received_length = strlen(received_as_string);

        if (received_length != expected_length) { return elara_false; }

        if (strncmp(expected, received_as_string, expected_length) == 0) {
            return ElaraTestResultPass;
        }
        return ElaraTestResultFail;
    },
    ^char *(void *received, char *to) {
        if (received == NULL) {
            char *message = calloc(31, 1);
            snprintf(message, 31, "Received NULL, expected char *");
            return message;
        }
        char *received_as_string = (char *)received;

        size_t message_length = 28 + strlen(to) + strlen(expected) + strlen(received_as_string);
        char *message = calloc(message_length, 1);
        snprintf(message, message_length, "Expected '%s' %s equal '%s'", received_as_string, to, expected);
        return message;
    });
}

ElaraMatcherReturn be_null() {
    return matcher_create(^ElaraTestResult(void *received) {
        if (received == NULL) {
            return ElaraTestResultPass;
        }
        return ElaraTestResultFail;
    },
    ^char *(void *received, char *to) {
        size_t message_length = 40 + strlen(to);
        char *message = calloc(message_length, 1);
        if (received == NULL) {
            snprintf(message, message_length, "Expected NULL to be null");
        } else {
            snprintf(message, message_length, "Expected %p %s be null", received, to);
        }
        return message;
    });
}

ElaraMatcherReturn be_truthy() {
    return matcher_create(^ElaraTestResult(void *received) {
        if (received == NULL) {
            return ElaraTestResultError;
        }
        if (*(int *)received != 0) {
            return ElaraTestResultPass;
        }
        return ElaraTestResultFail;
    },
    ^char *(void *received, char *to) {
        size_t message_length = 40 + strlen(to);
        char *message = calloc(message_length, 1);
        if (received == NULL) {
            snprintf(message, message_length, "Expected NULL %s be truthy", to);
        } else {
            snprintf(message, message_length, "Expected %p %s be truthy", received, to);
        }
        return message;
    });
}

#pragma mark - Expect

ExpectType elara_expect(void *received, const char *file, int line_number) {
    __block ExpectType value;

    value.to = Block_copy(^(ElaraMatcherReturn matcherValue) {
        assert(matcherValue.evaluator != NULL);
        elara_bool passed = matcherValue.evaluator(received);
        if (passed) {
            ElaraEnvironmentAssert(elara_true, NULL, file, line_number);
        } else {
            char *message = matcherValue.failure_message_formatter(received, "to");
            ElaraEnvironmentAssert(elara_false, message, file, line_number);
        }
        matcher_dealloc(matcherValue);
        Block_release(value.to_not);
        Block_release(value.to);
    });
    value.to_not = Block_copy(^(ElaraMatcherReturn matcherValue) {
        assert(matcherValue.evaluator != NULL);
        elara_bool passed = matcherValue.evaluator(received);
        if (!passed) {
            ElaraEnvironmentAssert(elara_true, NULL, file, line_number);
        } else {
            char *message = matcherValue.failure_message_formatter(received, "to not");
            ElaraEnvironmentAssert(elara_false, message, file, line_number);
        }
        matcher_dealloc(matcherValue);
        Block_release(value.to);
        Block_release(value.to_not);
    });
    return value;
}
