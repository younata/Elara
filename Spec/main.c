#include "Elara.h"

void ElaraSpec();
void StackSpec();
void TestContextSpec();

int main(int argc, char *argv[]) {
    ElaraSpec();
    StackSpec();
    TestContextSpec();
    return elara_main(argc, argv);
}
