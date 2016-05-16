#ifndef __TESTCONTEXT_H__
#define __TESTCONTEXT_H__

#include "List.h"
#include "Elara.h"

typedef enum test_status_enum {
    TestStatusNotATest,
    TestStatusNotRun,
    TestStatusFailed,
    TestStatusErrored,
    TestStatusSucceeded,
} TestStatus;

typedef struct {
    char *name;
    ElaraTestBlock block;
    TestStatus status;
    ElaraList *beforeEach;
    ElaraList *afterEach;
    ElaraList *children;
} TestContext;

TestContext *testContext_create(TestContext *parent_context);

void testContext_dealloc(TestContext *context);

#endif /* __TESTCONTEXT_H__ */
