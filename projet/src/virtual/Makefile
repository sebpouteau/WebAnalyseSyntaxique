
CC = gcc
CFLAGS = -std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ftest-coverage -fprofile-arcs -I../../include
LDLIBS = -lm -lgcov -lgraphviz 	-L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all: $(PATH_LIB)/libimport.a $(PATH_LIB)/libmachine.a 

$(PATH_LIB)/libimport.a: import.o machine.o pattern_matching.o $(PATH_INCLUDE)/pattern.h  $(PATH_INCLUDE)/xml_builder.h $(PATH_INCLUDE)/chemin.h  $(PATH_INCLUDE)/import.h  $(PATH_INCLUDE)/machine.h
	ar -cr $(PATH_LIB)/libimport.a import.o machine.o pattern_matching.o

$(PATH_LIB)/libmachine.a: machine.o pattern_matching.o ../graphviz/graphviz.o ../xml_builder/xml_builder.o $(PATH_INCLUDE)/*
	ar -cr $(PATH_LIB)/libmachine.a machine.o pattern_matching.o ../xml_builder/xml_builder.o ../graphviz/graphviz.o


.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* 



