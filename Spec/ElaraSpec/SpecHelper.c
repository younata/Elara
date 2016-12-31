#include "SpecHelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

ElaraMatcherReturn have_list_size(int expected_list_size) {
    return matcher_create(^elara_bool(void *received) {
        return elara_list_count((ElaraList *)received) == expected_list_size;
    },
    ^char *(void *received, char *to) {
        int received_list_count = elara_list_count((ElaraList *)received);

        size_t to_size = strlen(to);
        size_t expected_message_size = to_size + ceil(log10(received_list_count)) + ceil(log10(expected_list_size)) + 32;
        char *message = calloc(expected_message_size, 1);
        snprintf(message, expected_message_size, "expected list %s have size %d, got %d", to, received_list_count, expected_list_size);
        return message;
    });
}
