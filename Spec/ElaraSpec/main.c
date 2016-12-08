#include "Elara.h"

void ElaraSpec();
void ElaraListSpec();
void TestContextSpec();
void BeforeEachSpec();
void AfterEachSpec();
void TestReporterSpec();

int main(int argc, char *argv[]) {
    ElaraSpec();
    ElaraListSpec();
    TestContextSpec();
    BeforeEachSpec();
    AfterEachSpec();
    TestReporterSpec();

    return elara_main(argc, argv);
}
