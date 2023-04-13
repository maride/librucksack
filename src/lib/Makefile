.PHONY: all pack dist clean test

CFLAGS:=$(CFLAGS) -Wall -DLINUX -shared -fPIC

all: pack dist

pack:
	$(LD) --relocatable --format=binary -o pack.o pack

dist: pack
	$(CC) $(CFLAGS) -o librucksack.so $(wildcard *.c) ../common/common.o pack.o

test:
	$(MAKE) -C ../test dist
	LIBRUCKSACK_ENABLE_FILELIST=1 LIBRUCKSACK_ENABLE_TRACKLIST=1 LD_PRELOAD=./librucksack.so ../test/test_unistd /home/maride/.nmap/nselib/ssh1.lua
	LIBRUCKSACK_ENABLE_FILELIST=1 LIBRUCKSACK_ENABLE_TRACKLIST=1 LD_PRELOAD=./librucksack.so ../test/test_stdio /home/maride/.nmap/nselib/ssh1.lua

clean:
	rm -f *.so pack.o

