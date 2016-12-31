## Elara

[![Concourse](http://ci.younata.com/api/v1/teams/robotics/pipelines/robotics/jobs/elara_tests/badge)](https://ci.younata.com/teams/robotics/pipelines/robotics)
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
            expect("").to(be_truthy()); // True
        });

        describe("can also nest describes", ^{
            it("will run the nested it blocks", ^{
                expect(NULL).toNot(be_null()); // False
                // Nesting it blocks is not tested.
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

#### Matchers

Elara includes some barebones matchers, currently they are:

- `be_identical_to` (Tests on a pointer level whether two objects are actually equal)
- `equal` (Tests whether two ints are equal)
- `almost_equal` (Tests whether two floating point values are equal, to a given number of decimal places.
- `equal_string` (Tests whether two strings are equal)
- `be_null` (Tests whether an object is NULL or not)
- `be_truthy` (Tests whether something is equal on an integer level)

Making a custom matcher is simply just defining another function that returns an `ElaraMatcherReturn` object, which is a `struct` that has two members: an `evaluator` block and a `failure_message_formatter` block. The `evaluator` block takes in the received value (as a `void *`) and returns a char that is 0 if it failed, or 1 if it passed. The `failure_message_formatter` block takes in the received value (the value being tested), and a `to` string (this is either 'to' or 'to not'), and expects a 0-terminated-string of a message to present to the user in the event of failure.

There is a handy function `matcher_create` that will handle creating the ElaraMatcherReturn and properly assigning the `evaluator` and `failure_message_formatter` values (calling `Block_copy` on the arguments, as the blocks are released after being called).

### Building

Yeah, I spent forever on this too.

My solution was to make 2 makefiles - one for the project, the other for the specs. As a helper, I also added a "spec" tasks to the project makefile, so that I could just run `make spec` and the project would get built, then the specs would get built, then the specs would get run.

Another solution is to add your implementation and your spec files into the same target.

### Linux
Yes, Elara runs on linux. See the [circle.yml](circle.yml) file for how to set that up.

### Why?

There are no existing BDD frameworks for C that use blocks that I could find. Also, as a general teaching exercise for myself.