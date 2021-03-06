#ifndef __TESTCONTEXT_H__
#define __TESTCONTEXT_H__

#include "ElaraList.h"
#include "ElaraTypes.h"
#include "Elara.h"

typedef enum test_focused_enum {
    TestFocusUnfocused,
    TestFocusFocused,
    TestFocusSkipped,
} TestFocus;

typedef enum test_status_enum {
    TestStatusNotATest,
    TestStatusNotRun,
    TestStatusFailed,
    TestStatusErrored,
    TestStatusSucceeded,
    TestStatusSkipped,
} TestStatus;

typedef struct test_context_s {
    char *name;
    ElaraTestBlock block;
    TestStatus status;
    TestFocus focus;
    elara_bool has_focused_children;
    struct test_context_s *parent;
    char *message;
    ElaraList *beforeEach; // List<ElaraTestBlock>
    ElaraList *afterEach;
    ElaraList *children; // List<TestContext *>
} TestContext;

TestContext *testContext_create(TestContext *parent_context, TestFocus focus);

char *testContext_full_test_name(TestContext *context);

void testContext_run_beforeEachs(TestContext *context);
void testContext_run_afterEachs(TestContext *context);

void testContext_dealloc(TestContext *context);

#endif /* __TESTCONTEXT_H__ */
