
ALL_FILE = $(wildcard *.l)
MAIN=  $(ALL_FILE:.l=.c)
BISON = $(ALL_FILE:.l=.y)
DOT = $(ALL_FILE:.l=.dot)
PS = $(ALL_FILE:.l=.ps)
FILE = $(ALL_FILE:.l=.out)

CFLAGS=-std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes 

all: $(FILE) xml_builder

xml_builder:xml_builder.c tree.h tree.c
	gcc $^ $(CFLAGS) -o $@

%.out: %.tab.c %.yy.c tree.h tree.c
	gcc $^ $(MAIN)  -ly -lfl -lm $(CFLAGS) -o $@

%.tab.c: %.y
	bison -d -v $<

%.yy.c: %.l
	flex -o $@ $<

check:
	./$(FILE) < valid_forms.txt

draw:
	bison -d -v --graph $(BISON)
	dot -Tps $(DOT) -o $(PS)
	evince $(PS)


doc:
	firefox https://www.labri.fr/perso/zeitoun/enseignement/15-16/ASPP3/uploads/Main/HomePage/projet.html &

.PHONY: clean
clean:
	rm -rf *.output *.tab.* $(FILE) xml_builder *.yy.c *~ *.ps *.out* *.dot
