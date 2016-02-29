
ALL_FILE = $(wildcard *.l)
MAIN=  $(ALL_FILE:.l=.c)
BISON = $(ALL_FILE:.l=.y)
DOT = $(ALL_FILE:.l=.dot)
PS = $(ALL_FILE:.l=.ps)
FILE = $(ALL_FILE:.l=.out)
CFLAGS=-std=c99 -g -pedantic -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes 

all: $(FILE)

%.out: %.tab.c %.yy.c 
	gcc $^ -ly -ll -lm $(CFLAGS) -o $@

%.tab.c: %.y
	bison -d -v $<

%.yy.c: %.l
	flex -o $@ $<



draw:
	bison -d -v --graph $(BISON)
	dot -Tps $(DOT) -o $(PS)
	evince $(PS)

.PHONY: clean
clean:
	rm -f *.output *.tab.* $(FILE) *.yy.c *~
