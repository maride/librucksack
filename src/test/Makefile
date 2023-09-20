.PHONY: all dist clean

CFLAGS:=$(CFLAGS) -Wall -DLINUX -fPIC

all: clean dist

dist:
	$(CC) $(CFLAGS) -o test_stdio test_stdio.c
	$(CC) $(CFLAGS) -o test_unistd test_unistd.c
	$(CC) $(CFLAGS) -o test_stat test_stat.c

clean:
	rm -f test_stdio test_unistd test_stat

