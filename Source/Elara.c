#include "Elara.h"
#include <sys/queue.h>

TestContext baseContext;
TestContext *currentContext;

void elara_tests(ElaraTestBlock tests) {
    currentContext = &baseContext;
}

void describe(const char *name, ElaraTestBlock context) {
    TestContext *innerContext = calloc(1, sizeof(TestContext));
}

void it(const char *name, ElaraTestBlock test) {
    TestContext itContext;

    itContext.name = name;
    itContext.block = test;
    itContext.status = TestStatusNotRun;

    context_addSubcontext(currentContext, itContext);
}

void expect(int condition) {
    if (condition) {
    } else {
    }
}

void context_addSubcontext(TestContext *context, TestContext subcontext) {
    if (context->children == NULL) {
        context->children = calloc(1, sizeof(TestContext));
    } else if (sizeof(context->children) / sizeof(TestContext) == context->childrenCount) {
        int size = sizeof(TestContext) * context->childrenCount * 2;
        context->children = realloc(context->children, size);
    }

    context->children[context->childrenCount] = subcontext;

    context->childrenCount += 1;
}

int main(int argc, char *argv[]) {
    return 0;
}
