
ALL_FILE = $(wildcard *.l)
MAIN=  $(ALL_FILE:.l=.c)
BISON = $(ALL_FILE:.l=.y)
DOT = $(ALL_FILE:.l=.dot)
PS = $(ALL_FILE:.l=.ps)
FILE = $(ALL_FILE:.l=.out)

PATH_INCLUDE=../../include
PATH_LIB=../../lib
PATH_BIN=../../bin

CFLAGS=-std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -ftest-coverage -fprofile-arcs -I../../include 
LDLIBS= -ltree -lxml -limport -lutil -lqueue -L../../lib/

all: $(PATH_BIN)/$(FILE) 

$(PATH_BIN)/%.out: %.tab.c %.yy.c  $(PATH_LIB)/*
	gcc $^ $(MAIN) -ly -ll -lm $(CFLAGS) -o  $@

%.tab.c: %.y
	bison -d -v $<

%.yy.c: %.l
	flex -o $@ $<


draw:
	bison -d -v --graph $(BISON)
	dot -Tps $(DOT) -o $(PATH_BIN)/$(PS)
	evince $(PATH_BIN)/$(PS)


doc:
	firefox https://www.labri.fr/perso/zeitoun/enseignement/15-16/ASPP3/uploads/Main/HomePage/projet.html &

.PHONY: clean
clean:
	rm -rf *.output *.tab.* $(FILE) xml_builder *.yy.c *~ *.ps *.out* *.dot *~ *.g* 
