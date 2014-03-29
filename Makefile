uname := $(shell uname)

ifeq ($(uname),Linux)
        CC = gcc
        CFLAGS = -g -O3 -lpthread
endif
ifeq ($(uname),SunOS)
        CXX = CC
        CXXFLAGS = -g -mt -D_LARGEFILE_SUPPORT -D_LARGEFILE64_SUPPORT
        LIBS = -lrt -lpthread
endif

## If we assume that you have 3 files: poker.c poker.h play_poker.c
## And you have to compile all the files together for creating vsh, then
## change the below line in the Makefile to reflect compiling your code
OBJS=play_poker.c poker.c

.PHONY: all
all: play_poker

play_poker: $(OBJS)
	$(LINK.c) -o $@ $^ $(LIBS)

clean:
	rm -f *.o core core.* play_poker

# Just make all .o file depend on all header files.
srcs := $(wildcard *.c)
objs := $(srcs:.c=.o)
$(objs):= *.h

