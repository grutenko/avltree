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
LD_INCLUDE_PATH =
LIBS =
CFLAGS = -O2 -Winline

.PHONY: all

all: test

%.out:
	$(CC) $(LD_INCLUDE_PATH) $(LIBS) -target $(TARGET) $(CFLAGS) -o $@ $^

%.$(OBJEXT): %.c
	$(CC) -c $(LD_INCLUDE_PATH) -target $(TARGET) $(CFLAGS) $^ -o $@

.PHONY: test compile-test

test: compile-test
	./avltreetest.out

compile-test: avltreetest.out

avltreetest.out: avltreetest.$(OBJEXT) avltree.$(OBJEXT)

%test.$(OBJEXT): LD_INCLUDE_PATH := $(LD_INCLUDE_PATH) $(shell pkg-config --cflags check)

%test.out: LIBS := $(LIBS) $(shell pkg-config --libs check) -lm -lpthread
%test.out: %test.$(OBJEXT)
	$(CC) $(LD_INCLUDE_PATH) $(LIBS) -target $(TARGET) $(CFLAGS) -o $@ $^

.PHONY: clean clean-test

clean:
	rm -f *.$(OBJEXT) *.out

clean-test:
	rm -f *test.$(OBJEXT) *test.out

