LIBS= abstract  virtual graphviz parser

all:
	for i in $(LIBS);do\
		$(MAKE) -C $$i all;\
	done

draw:
	$(MAKE) -C parser draw
clean:
	for i in $(LIBS);do\
		$(MAKE) -C $$i clean;\
	done





