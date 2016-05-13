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

.PHONY: clean spec

spec: Elara
	make -f Makefile.test
	./ElaraSpec

debugspec: Elara
	make -f Makefile.test
	lldb ElaraSpec

clean:
	rm -rf Source/*.o *.so Elara/*
	make -f Makefile.test clean
