
CC = gcc
CFLAGS = -std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ftest-coverage -fprofile-arcs -I../../include
LDLIBS = -lm -lgcov -L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all: $(PATH_LIB)/libpattern.a $(PATH_LIB)/libpast.a 


$(PATH_LIB)/libpattern.a: pattern.o pattern.c $(PATH_INCLUDE)/pattern.h  $(PATH_INCLUDE)/chemin.h
	ar -cr $(PATH_LIB)/libpattern.a pattern.o

$(PATH_LIB)/libpast.a: ast.o ast.c $(PATH_INCLUDE)/ast.h  $(PATH_INCLUDE)/chemin.h
	ar -cr $(PATH_LIB)/libast.a ast.o


.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* 



