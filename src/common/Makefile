.PHONY: all pack dist clean

CFLAGS:=$(CFLAGS) -Wall -DLINUX -ldl -c -fPIC

all: pack dist

dist:
	$(CC) $(CFLAGS) -o common.o $(wildcard *.c) 

clean:
	rm -f *.o

