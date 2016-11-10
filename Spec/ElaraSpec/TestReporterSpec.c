#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "Elara.h"
#include "List.h"
#include "TestReporter.h"

void TestReporterSpec() {
    describe("Test Reporters", ^{
        describe("testReport_add_report", ^{
            it("adds items to the test report list when they're failed, errored, or succeeded", ^{
                ElaraList *list = elara_list_create();

                testReport_add_report(list, "test name", "not set", TestStatusSucceeded, 3.0);
                testReport_add_report(list, "test name 2", "failed", TestStatusFailed, 2.0);
                testReport_add_report(list, "test name 3", "errored", TestStatusErrored, 4.0);

                expect(elara_list_count(list) == 3);

                TestReport *report = elara_list_get(list, 0);
                expect(report != NULL);
                expect(strncmp(report->name, "test name", 10) == 0);
                expect(report->message == NULL);
                expect(report->result == TestStatusSucceeded);
                expect(report->runtime == 3.0);

                report = elara_list_get(list, 1);
                expect(report != NULL);
                expect(strncmp(report->name, "test name 2", 12) == 0);
                expect(strncmp(report->message, "failed", 7) == 0);
                expect(report->result == TestStatusFailed);
                expect(report->runtime == 2.0);

                report = elara_list_get(list, 2);
                expect(report != NULL);
                expect(strncmp(report->name, "test name 3", 12) == 0);
                expect(strncmp(report->message, "errored", 8) == 0);
                expect(report->result == TestStatusErrored);
                expect(report->runtime == 4.0);

                elara_list_dealloc(list, ^(void *entry) {
                    if (entry != NULL) {
                        free(entry);
                    }
                });
            });

            it("does not add any test reports with a status of NotATest", ^{
                ElaraList *list = elara_list_create();

                testReport_add_report(list, "test name", NULL, TestStatusNotATest, 3.0);

                expect(elara_list_count(list) == 0);

                elara_list_dealloc(list, ^(void *entry) {
                    if (entry != NULL) {
                        free(entry);
                    }
                });
            });

            it("does not add any test reports with a status of NotRun", ^{
                ElaraList *list = elara_list_create();

                testReport_add_report(list, "test name", NULL, TestStatusNotRun, 3.0);

                expect(elara_list_count(list) == 0);

                elara_list_dealloc(list, ^(void *entry) {
                    if (entry != NULL) {
                        free(entry);
                    }
                });
            });
        });

        describe("testReport_create_report", ^{
            it("correctly outputs xunit style xml", ^{
                FILE *test_output = fopen("test.out", "w+");
                expect(test_output != NULL);
                if (test_output == NULL) {
                    return;
                }

                ElaraList *list = elara_list_create();
                testReport_add_report(list, "test name", NULL, TestStatusSucceeded, 3.0);
                testReport_add_report(list, "test name 2", "expected 2 to be 3", TestStatusFailed, 2.0);
                testReport_add_report(list, "test name 3", "unknown error", TestStatusErrored, 4.0);

                testReport_create_report(list, TestReportStyleXUnit, test_output);

                fseek(test_output, 0, SEEK_END);
                long received_length = ftell(test_output);
                fseek(test_output, 0, SEEK_SET);
                char *received_output = (char *)calloc(received_length, 1);
                fread(received_output, 1, received_length, test_output);
                fclose(test_output);

                FILE *fixture = fopen("Spec/Fixtures/testReport_xunit.xml", "r");
                expect(fixture != NULL);
                if (fixture == NULL) {
                    free(received_output);
                    return;
                }

                fseek(fixture, 0, SEEK_END);
                long fixture_length = ftell(fixture);
                fseek(fixture, 0, SEEK_SET);
                char *fixture_contents = (char *)calloc(fixture_length, 1);
                fread(fixture_contents, 1, fixture_length, test_output);
                fclose(fixture);

                expect(received_length == fixture_length);
                expect(strncmp(received_output, fixture_contents, fixture_length) == 0);

                free(received_output);
                free(fixture_contents);

                unlink("test.out");
            });
        });
    });
}