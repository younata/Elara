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

typedef struct test_context_s {
    char *name;
    ElaraTestBlock block;
    TestStatus status;
    struct test_context_s *parent;
    ElaraList *beforeEach; // List<ElaraTestBlock>
    ElaraList *afterEach;
    ElaraList *children; // List<TestContext *>
} TestContext;

TestContext *testContext_create(TestContext *parent_context);

void testContext_run_beforeEachs(TestContext *context);

void testContext_dealloc(TestContext *context);

#endif /* __TESTCONTEXT_H__ */
