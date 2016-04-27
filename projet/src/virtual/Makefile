
CC = gcc
CFLAGS = -std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ftest-coverage -fprofile-arcs -I../../include
LDLIBS = -lm -lgcov -L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all: $(PATH_LIB)/libimport.a $(PATH_LIB)/libmachine.a 

$(PATH_LIB)/libimport.a: import.o $(PATH_INCLUDE)/pattern.h  $(PATH_INCLUDE)/chemin.h
	ar -cr $(PATH_LIB)/libimport.a import.o

$(PATH_LIB)/libmachine.a: machine.o pattern_matching.o $(PATH_INCLUDE)/ast.h  $(PATH_INCLUDE)/chemin.h
	ar -cr $(PATH_LIB)/libmachine.a machine.o pattern_matching.o


.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* 



