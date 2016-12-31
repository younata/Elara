#include "Matchers.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <Block.h>

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
    return matcher_create(^elara_bool(void *received) {
        return expected == received;
    },
    ^char *(void *received, char *to) {
        size_t message_length = 70 + strlen(to);
        char *message = calloc(message_length, 1);
        snprintf(message, message_length, "Expected %p %s be same address as %p", expected, to, received);
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

    return matcher_create(^elara_bool(void *received) {
        int value = *(int *)received;
        return value == expected;
    },
    ^char *(void *received, char *to) {
        int value = *(int *)received;

        size_t expected_message_size = strlen(to) + decimal_places_in_int(value) +
            decimal_places_in_int(expected) + 20;
        char *message = calloc(expected_message_size, 1);
        snprintf(message, expected_message_size, "expected %d %s equal %d", value, to, expected);

        return message;
    });
}

ElaraMatcherReturn almost_equal(double expected, double decimal_places) {
    return matcher_create(^elara_bool(void *received) {
        double value = *(double *)received;

        return fabs(value - expected) < pow(10, -decimal_places);
    },
    ^char *(void *received, char *to) {
        double value = *(double *)received;

        char *message = calloc(1024, 1);
        snprintf(message, 1024, "Expected %f %s equal %f within %f decimal places", value, to, expected, decimal_places);
        return message;
    });
}

ElaraMatcherReturn equal_string(char *expected) {
    size_t expected_length = strlen(expected);

    return matcher_create(^elara_bool(void *received) {
        char *received_as_string = (char *)received;

        size_t received_length = strlen(received_as_string);

        if (received_length != expected_length) { return elara_false; }

        return strncmp(expected, received_as_string, expected_length) == 0;
    },
    ^char *(void *received, char *to) {
        char *received_as_string = (char *)received;

        size_t message_length = 28 + strlen(to) + strlen(expected) + strlen(received_as_string);
        char *message = calloc(message_length, 1);
        snprintf(message, message_length, "Expected '%s' %s equal '%s'", received_as_string, to, expected);
        return message;
    });
}

ElaraMatcherReturn be_null() {
    return matcher_create(^elara_bool(void *received) {
        return received == NULL;
    },
    ^char *(void *received, char *to) {
        size_t message_length = 40 + strlen(to);
        char *message = calloc(message_length, 1);
        snprintf(message, message_length, "Expected %p %s be null", received, to);
        return message;
    });
}

ElaraMatcherReturn be_truthy() {
    return matcher_create(^elara_bool(void *received) {
        return *(int *)received != 0;
    },
    ^char *(void *received, char *to) {
        size_t message_length = 40 + strlen(to);
        char *message = calloc(message_length, 1);
        snprintf(message, message_length, "Expected %p %s be truthy", received, to);
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
        Block_release(value.toNot);
        Block_release(value.to);
    });
    value.toNot = Block_copy(^(ElaraMatcherReturn matcherValue) {
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
        Block_release(value.toNot);
    });
    return value;
}
