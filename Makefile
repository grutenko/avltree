ifeq ($(OS),Windows_NT) # OS is a preexisting environment variable on Windows
	OS = windows
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME),Darwin)
		OS = macos
	else ifeq ($(UNAME),Linux)
		OS = linux
	else
    	$(error OS not supported by this Makefile)
	endif
endif

ifeq ($(OS),windows)
    OBJEXT := obj
		EXEEXT := .exe
else
    OBJEXT := o
		EXEEXT :=
endif

CC = /usr/bin/gcc
TARGET = arm64-apple-darwin
CFLAGS = -O2 -Winline

.PHONY: all

all: test

%.out:
	$(CC) -target $(TARGET) $(CFLAGS) -o $@ $^

%.$(OBJEXT): %.c
	$(CC) -c -target $(TARGET) $(CFLAGS) $^ -o $@

.PHONY: test compile-test

test: compile-test
	./avltreetest.out

compile-test: avltreetest.out

.PHONY: bench compile-bench

bench: compile-bench

compile-bench: avltreebench.out

avltreebench.out: avltreebench.$(OBJEXT) avltree.$(OBJEXT)

%bench.out: %bench.$(OBJEXT)
	$(CC) -target $(TARGET) $(CFLAGS) -o $@ $^

avltreetest.out: avltreetest.$(OBJEXT) avltree.$(OBJEXT)
%test.out: %test.$(OBJEXT)
	$(CC) -target $(TARGET) $(CFLAGS) -o $@ $^

.PHONY: clean clean-test

clean:
	rm -f *.$(OBJEXT) *.out

clean-test:
	rm -f *test.$(OBJEXT) *test.out

