#ifndef __TESTCONTEXT_H__
#define __TESTCONTEXT_H__

#include "Stack.h"

typedef enum {
    TestStatusNotATest;
    TestStatusNotRun;
    TestStatusFailed;
    TestStatusErrored;
    TestStatusSucceeded;
} TestStatus;

typedef struct {
    char *name;
    ElaraTestBlock block;
    TestStatus status;
    ElaraStack *children;
} TestContext;

TestContext *testContext_create(TestContext *parent_context);
void testContext_dealloc(TestContext *context);

#endif /* __TESTCONTEXT_H__ */
