CC=clang

C_SRC=$(wildcard Source/*.c)
CFLAGS=-Wall -Werror -fblocks -fPIC -IInclude -g

SHAREDLIB=libelara.so

DEPS=Include/*.h

OBJ=$(C_SRC:.c=.o)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Elara: $(OBJ)
	mkdir -p Elara
	$(CC) -shared -o Elara/$(SHAREDLIB) $^ $(CFLAGS)
	cp Include/Elara.h Elara
	cp Include/ElaraTypes.h Elara
	cp Include/Matchers.h Elara

.PHONY: clean spec circlespec elaraspec elaradebugspec focusedspec focuseddebugspec fly integration

spec: Elara
	make -f Makefile.test
	./ElaraSpec
	make -f Makefile.focusedtest
	./ElaraFocusedSpec

circlespec: Elara
	mkdir $(CIRCLE_TEST_REPORTS)/elara
	make -f Makefile.test
	./ElaraSpec -x $(CIRCLE_TEST_REPORTS)/elara/main.xml
	make -f Makefile.focusedtest
	./ElaraFocusedSpec -x $(CIRCLE_TEST_REPORTS)/elara/focused.xml

elaraspec: Elara
	make -f Makefile.test
	./ElaraSpec

elaradebugspec: Elara
	make -f Makefile.test
	lldb ElaraSpec

focusedspec: Elara
	make -f Makefile.focusedtest
	./ElaraFocusedSpec

focuseddebugspec: Elara
	make -f Makefile.focusedtest
	lldb ElaraFocusedSpec

clean:
	rm -rf Source/*.o *.so Elara/*
	make -f Makefile.test clean
	make -f Makefile.focusedtest clean

fly: clean
	fly -t ci execute --config ./concourse/tests.yml --input elara_github=.

integration:
	bats Spec/Integration/elara.bats
