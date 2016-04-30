LIBS= src
TESTS=$(wildcard src/check/valid_forms/*.txt)
TESTS2=$(wildcard src/check/invalid_forms/*.txt)
PAGE=$(wildcard src/check/pageHTML/*.txt)

all:
	if [ ! -d "bin" ];then mkdir bin; fi
	if [ ! -d "lib" ];then mkdir lib; fi
	$(MAKE) -C $(LIBS) all	



check: all
	echo -e "\n\n\033[32mVALIDE FORME\033[0m"
	for i in $(TESTS); do \
		echo -n  "$$i ... "; \
		eval "./bin/parser.out <$$i" > log 2>&1; \
		if test $$? -eq 0; then \
			echo "\033[32mPASS\033[0m"; \
		else \
			echo "\033[31mFAIL\033[0m" && echo "" && cat log && echo ""; \
		fi \
	done

	echo -e "\n\n\033[31mINVALIDE FORME\033[0m"
	for i in $(TESTS2); do \
		echo -n  "$$i ... "; \
		eval "./bin/parser.out <$$i" > log 2>&1; \
		if test $$? -eq 0; then \
			echo -e "\033[32mPASS\033[0m" && echo "" && cat log && echo ""; \
		else \
			echo -e "\033[31mFAIL\033[0m"; \
		fi \
	done
	if [ ! -d "bin/check" ];then mkdir bin/check; fi
	if [ ! -d "bin/check/html" ];then mkdir bin/check/html; fi
	if [ ! -d "bin/check/graph" ];then mkdir bin/check/graph; fi
	mv *.html bin/check/html
	mv *.dot bin/check/graph

	echo -e "\n\n\033[32mCONVERT TO PNG\033[0m"

	for i in bin/check/graph/*.dot; do \
		echo -n -e "$$i ... wait ... "; \
		dot -Tpng $$i -o $$i.png; \
		echo -e "\033[32mFINISH\033[0m result: $$i.png" ; \
	done
	gcov src/graphviz/graphviz.c
	gcov src/util/util.c
	gcov src/util/queue.c
	gcov src/xml_builder/xml_builder.c
	gcov src/virtual/machine.c
	gcov src/abstract/ast.c
	rm *.gcov log



page: all
	echo "\n\n\033[32mGENERATION DES PAGE HTML\033[0m"
	for i in $(PAGE); do \
		echo -n -e "$$i ... "; \
		eval "./bin/parser.out <$$i"; \
	done
	if [ ! -d "bin/html" ];then mkdir bin/html; fi
	if [ ! -d "bin/graph" ];then mkdir bin/graph; fi
	mv *.html bin/html
	mv *.dot bin/graph
	for i in bin/graph/*.dot; do \
		echo -n -e "$$i ... wait"; \
		dot -Tpng $$i -o $$i.png; \
		echo -e "\033[32mFINISH\033[0m result: $$i.png" ; \
	done


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
