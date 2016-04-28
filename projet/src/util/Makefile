
CC = gcc
CFLAGS = -std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ftest-coverage -fprofile-arcs -I../../include
LDLIBS = -lm -lgcov -L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all:  $(PATH_LIB)/libqueue.a 

 

$(PATH_LIB)/libqueue.a: queue.o $(PATH_INCLUDE)/queue.h queue.c
	ar -cr $(PATH_LIB)/libqueue.a queue.o


.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* 



