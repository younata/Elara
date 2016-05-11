CC=clang

C_SRC=$(wildcard Source/*.c)
CFLAGS=-fblocks -ISource

SHAREDLIB=elara.so

DEPS=Source/*.h

OBJ=$(C_SRC:.c=.o)

%.o: %.c $(DEPS) 
	$(CC) -c -o $@ $< $(CFLAGS)

elara: $(OBJ)
	$(CC) -shared -o $(SHAREDLIB) $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -rf Source/*.o
