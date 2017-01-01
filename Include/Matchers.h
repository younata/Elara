#ifndef __MATCHERS_H__
#define __MATCHERS_H__

#include "ElaraTypes.h"

typedef char *(^ElaraMatcherFormatter)(void *, char *);
typedef elara_bool (^ElaraMatcherEvaluator)(void *);

typedef struct {
    ElaraMatcherEvaluator evaluator;
    ElaraMatcherFormatter failure_message_formatter;
} ElaraMatcherReturn;

typedef void (^ElaraExpectDSLBlock)(ElaraMatcherReturn);

typedef struct {
    ElaraExpectDSLBlock to;
    ElaraExpectDSLBlock toNot; // inverts the matcher
} ExpectType;

ElaraMatcherReturn matcher_create(ElaraMatcherEvaluator evaluator, ElaraMatcherFormatter formatter);

ElaraMatcherReturn be_identical_to(void *expected);
ElaraMatcherReturn equal(int expected);
ElaraMatcherReturn almost_equal(double expected, double decimal_places);
ElaraMatcherReturn equal_string(char *expected);
ElaraMatcherReturn be_null();
ElaraMatcherReturn be_truthy();

ExpectType elara_expect(void *condition, const char *file, int line_number);
#define expect(given)    elara_expect(given, __FILE__, __LINE__)

// Global environment variable

void setElaraEnvironmentAssert(void (*elara_assert)(elara_bool, char *, const char *, int));

#endif /* __MATCHERS_H__ */
