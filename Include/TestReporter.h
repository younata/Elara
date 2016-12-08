#ifndef __TESTREPORTER_H__
#define __TESTREPORTER_H__

#include "ElaraList.h"
#include "TestContext.h"

typedef struct test_report_s {
    char *name;
    char *message;
    TestStatus result;
    double runtime;
} TestReport;

typedef enum test_report_enum {
    TestReportStyleRSpec,
    TestReportStyleXUnit,
} TestReportStyle;

void testReport_add_report(ElaraList *reports, char *test_name, char *message, TestStatus result, double test_runtime);
void testReport_create_report(ElaraList *reports, TestReportStyle reportStyle, FILE *output);

#endif /* __TESTREPORTER_H__ */
