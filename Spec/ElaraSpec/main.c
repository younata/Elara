#include "Elara.h"

void ElaraSpec();
void ElaraListSpec();
void TestContextSpec();
void BeforeEachSpec();
void AfterEachSpec();
void TestReporterSpec();
void MatcherSpec();

int main(int argc, char *argv[]) {
    ElaraSpec();
    ElaraListSpec();
    TestContextSpec();
    BeforeEachSpec();
    AfterEachSpec();
    TestReporterSpec();
    MatcherSpec();

    return elara_main(argc, argv);
}
