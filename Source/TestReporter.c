#include <stdio.h>
#include <stdlib.h>

#include "TestReporter.h"
#include "List.h"

void testReport_create_report_xunit(ElaraList *reports, FILE *output);

void testReport_add_report(ElaraList *reports, char *test_name, char *message, TestStatus result, double test_runtime) {
    if (result == TestStatusNotATest || result == TestStatusNotRun) {
        return;
    }
    TestReport *report = calloc(1, sizeof(TestReport));
    report->name = test_name;
    switch (result) {
        case TestStatusSucceeded:
        case TestStatusSkipped:
            break;
        default:
            report->message = message;
            break;
    }
    report->result = result;
    report->runtime = test_runtime;

    elara_list_append(reports, report);
}

void testReport_create_report(ElaraList *reports, TestReportStyle reportStyle, FILE *output) {
    switch (reportStyle) {
    case TestReportStyleXUnit:
        testReport_create_report_xunit(reports, output);
        break;
    case TestReportStyleRSpec:
        break;
    }
}

void testReport_create_report_xunit(ElaraList *reports, FILE *output) {
    fprintf(output, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<testsuite>\n");
    elara_list_foreach(reports, ^(void *object) {
        TestReport *report = (TestReport *)object;
        if (report->result == TestStatusSucceeded) {
            fprintf(output, "    <testcase name=\"%s\" time=\"%f\" />\n", report->name, report->runtime);
        } else {
            fprintf(output, "    <testcase name=\"%s\" time=\"%f\">\n", report->name, report->runtime);
            if (report->result == TestStatusFailed) {
                fprintf(output, "        <failure message=\"%s\" />\n", report->message);
            } else if (report->result == TestStatusErrored) {
                fprintf(output, "        <error message=\"%s\" />\n", report->message);
            } else if (report->result == TestStatusSkipped) {
                fprintf(output, "        <skipped />\n");
            }
            fprintf(output, "    </testcase>\n");
        }
    });
    fprintf(output, "</testsuite>\n");
}
