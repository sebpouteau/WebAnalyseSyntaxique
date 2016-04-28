CC = gcc
CFLAGS = -std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ftest-coverage -fprofile-arcs -I../../include
LDLIBS = -lm -lgcov -lxml -L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all: $(PATH_LIB)/libxml.a 

$(PATH_LIB)/libxml.a: xml_builder.o xml_builder.c $(PATH_INCLUDE)/xml_builder.h
	ar -cr $(PATH_LIB)/libxml.a xml_builder.o

.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* 

dep:
	$(CC) $(CPPFLAGS) -MM *.c >dep


