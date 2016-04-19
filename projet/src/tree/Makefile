
CC = gcc
CFLAGS = -std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ftest-coverage -fprofile-arcs -I../../include
LDLIBS = -lm -lgcov -L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all: $(PATH_LIB)/libtree.a 
 
$(PATH_LIB)/libtree.a: tree.o tree.c $(PATH_INCLUDE)/tree.h
	ar -cr $(PATH_LIB)/libtree.a tree.o


.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* 



