CC ?= gcc
CFLAGS ?= -std=gnu99

EXEC = shell 
SRC_COMMON = io.c

default: io.o
	$(CC) $(CFLAGS) io.o shell.c -o shell 

.PHONY: clean
clean:
	rm $(EXEC) *.o 
