#include "Elara.h"

void ElaraSpec();
void ListSpec();
void TestContextSpec();
void BeforeEachSpec();

int main(int argc, char *argv[]) {
    ElaraSpec();
    ListSpec();
    TestContextSpec();
    BeforeEachSpec();
    return elara_main(argc, argv);
}
