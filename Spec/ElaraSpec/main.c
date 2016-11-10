#include "Elara.h"

void ElaraSpec();
void ListSpec();
void TestContextSpec();
void BeforeEachSpec();
void AfterEachSpec();
void TestReporterSpec();

int main(int argc, char *argv[]) {
    ElaraSpec();
    ListSpec();
    TestContextSpec();
    BeforeEachSpec();
    AfterEachSpec();
    TestReporterSpec();

    return elara_main(argc, argv);
}
