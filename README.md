## Elara

[![CircleCi](https://circleci.com/gh/younata/Elara.png?style=shield)](https://circleci.com/gh/younata/Elara)

BDD-Style test suite for C.

Named after the [Moon of Jupiter](https://en.wikipedia.org/wiki/Elara_(moon))

[Tracker Project for Elara](https://www.pivotaltracker.com/n/projects/1588925)

License: [MIT](LICENSE)

### Syntax/DSL

Elara defines a nice little DSL for running specs. At its most basic, a test can look something like

```C
#include "Elara.h"
void MySpec() {
    describe("A set of tests", ^{
        it("runs a single test", ^{
            expect(1); // True
        });
        
        describe("can also nest describes", ^{
            it("will run the nested it blocks", ^{
                expect(0); // False
                // Nesting it blocks is undefined behavior.
                // Most likely, nothing will happen, but no guarantees.
            });
        });
    });
}

int main(int argc, char *argv[]) {
    MySpec();
    elara_main(argc, argv);
}
```

See the [Spec](Spec) folder for more examples.

### Building

Yeah, I spent forever on this too.

My solution was to make 2 makefiles - one for the project, the other for the specs. As a helper, I also added a "spec" tasks to the project makefile, so that I could just run `make spec` and the project would get built, then the specs would get built, then the specs would get run.

Also, it appears to Elara specs work best when your project is a shared library. This also has the side benefit of forcing you to not have a giant `main()` function.

### Linux
Yes, Elara runs on linux. See the [circle.yml](circle.yml) file for how to set that up.

### Why?

There are no existing BDD frameworks for C that use blocks that I could find. Also, as a general teaching exercise for myself.

Following my love of astronomy, Elara is named after a [Moon of Jupiter](https://en.wikipedia.org/wiki/Elara_(moon)).



