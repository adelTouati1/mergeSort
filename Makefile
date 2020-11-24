
CC=gcc
CFLAGS=-g -std=gnu89 -Wall -Wextra -Wpointer-arith -Wstrict-prototypes -I. 
# specify libraries to compile below, e.g. -lm (for the math library)
LIBS=-lpthread

mytests: mytests.o mergesort.o timing.o 
	$(CC) $(CPPFLAGS)  mytests.o mergesort.o timing.o $(LIBS) -o mytests

clean:
	/bin/rm -f *.o mytests
