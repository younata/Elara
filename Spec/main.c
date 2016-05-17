#include "Elara.h"

void ElaraSpec();
void ListSpec();
void TestContextSpec();
void BeforeEachSpec();
void AfterEachSpec();

int main(int argc, char *argv[]) {
    ElaraSpec();
    ListSpec();
    TestContextSpec();
    BeforeEachSpec();
    AfterEachSpec();

    return elara_main(argc, argv);
}
