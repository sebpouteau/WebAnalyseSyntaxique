

all: graphviz

graphviz: graphviz.c ast.c
	gcc -std=c99 graphviz.c ast.c -o graphviz
	./graphviz
	dot -Tpng fichier.dot -o fichier.png
	xdg-open fichier.png

.PHONY: clean
clean:
	rm -f dep *.o *~ *.g* graphviz fichier.dot fichier.png



