LIBS= src
TESTS=$(wildcard src/check/valid_forms/*.txt)
TESTS2=$(wildcard src/check/invalid_forms/*.txt)

all:
	if [ ! -d "bin" ];then mkdir bin; fi
	if [ ! -d "lib" ];then mkdir lib; fi
	$(MAKE) -C $(LIBS) all	



check:all
	echo "\n\n\033[32mVALIDE FORME\033[0m"
	for i in $(TESTS); do \
		echo -n  "$$i ... "; \
		eval "./bin/parser.out <$$i" > log 2>&1; \
		if test $$? -eq 0; then \
			echo "\033[32mPASS\033[0m"; \
		else \
			echo "\033[31mFAIL\033[0m" && echo "" && cat log && echo ""; \
		fi \
	done

	echo "\n\n\033[31mINVALIDE FORME\033[0m"
	for i in $(TESTS2); do \
		echo -n  "$$i ... "; \
		eval "./bin/parser.out <$$i" > log 2>&1; \
		if test $$? -eq 0; then \
			echo "\033[32mPASS\033[0m" && echo "" && cat log && echo ""; \
		else \
			echo "\033[31mFAIL\033[0m"; \
		fi \
	done
	gcov src/graphviz/graphviz.c
	gcov src/util/util.c
	gcov src/util/queue.c
	gcov src/xml_builder/xml_builder.c
	gcov src/virtual/machine.c
	gcov src/abstract/ast.c

draw:all
	$(MAKE) -C $(LIBS) draw

clean:
	$(MAKE) -C $(LIBS) clean;\
	rm -rf *.db *.g* log *.dot *.html
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi
	if [ -d "html" ];then rm -rf html; fi
	if [ -d "latex" ];then rm -rf latex; fi
doc: 
	doxygen Doxyfile
