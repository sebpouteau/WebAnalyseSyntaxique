
CC = gcc
CFLAGS = -std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ftest-coverage -fprofile-arcs -I../../include
LDLIBS = -lm -last -lgraphviz -L ../../lib/
CPPFLAGS= 

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

all: $(PATH_LIB)/libgraphviz.a graphviz


$(PATH_LIB)/libgraphviz.a: graphviz.o graphviz.c $(PATH_INCLUDE)/graphviz.h
	ar -cr $(PATH_LIB)/libgraphviz.a  graphviz.o 


graphviz: main.c $(PATH_LIB)/libgraphviz.a
	gcc $(CFLAGS) main.c graphviz.o $(LDLIBS) -o graphviz
	./graphviz
	dot -Tpng fichier.dot -o fichier.png


.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* graphviz fichier.dot fichier.png




