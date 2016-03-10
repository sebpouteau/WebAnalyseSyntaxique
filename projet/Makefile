LIBS= src

all:
	if [ ! -d "bin" ];then mkdir bin; fi
	if [ ! -d "lib" ];then mkdir lib; fi
	$(MAKE) -C $(LIBS) all	



check:all
	valgrind ./bin/parser.out < src/check/valid_forms.txt
	gcov src/tree/tree.c
	gcov src/xml_builder/xml_builder.c

draw:all
	$(MAKE) -C $(LIBS) draw

clean:
	$(MAKE) -C $(LIBS) clean;\
	rm -rf *.db *.g*
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi
	if [ -d "html" ];then rm -rf html; fi
	if [ -d "latex" ];then rm -rf latex; fi
doc: 
	doxygen Doxyfile
