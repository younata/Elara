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

.PHONY: clean spec elaraspec elaradebugspec focusedspec focuseddebugspec fly

spec: Elara
	make -f Makefile.test
	./ElaraSpec
	make -f Makefile.focusedtest
	./ElaraFocusedSpec

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
