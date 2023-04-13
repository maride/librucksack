.PHONY: all dist clean test

all: dist test

dist:
	$(MAKE) -C src/common dist
	$(MAKE) -C src/lib dist
	$(MAKE) -C src/test dist

clean:
	$(MAKE) -C src/common clean
	$(MAKE) -C src/lib clean
	$(MAKE) -C src/test clean

test:
	$(MAKE) -C src/lib test
