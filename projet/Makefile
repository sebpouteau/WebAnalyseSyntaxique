
LIBS= src
TESTS=$(wildcard src/check/valid_forms/*.txt)
TESTS2=$(wildcard src/check/invalid_forms/*.txt)
PAGE=$(wildcard src/check/pageHTML/*.txt)

all:
	if [ ! -d "bin" ];then mkdir bin; fi
	if [ ! -d "lib" ];then mkdir lib; fi
	$(MAKE) -C $(LIBS) all	



check: all
	echo -e "\n\n\033[32mVALID FORMS\033[0m"
	for i in $(TESTS); do \
		echo -n  "$$i ... "; \
		eval "./bin/parser.out <$$i" > log 2>&1; \
		if test $$? -eq 0; then \
			echo "\033[32mPASS\033[0m"; \
		else \
			echo "\033[31mFAIL\033[0m" && echo "" && cat log && echo ""; \
		fi \
	done

	echo -e "\n\n\033[31mINVALID FORMS\033[0m"
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

	echo -e "\n\n\033[32mCONVERTING TO PNG\033[0m"

	for i in bin/check/graph/*.dot; do \
		echo -n -e "$$i ... wait ... "; \
		dot -Tpng $$i -o $$i.png; \
		echo -e "\033[32mDONE\033[0m result: $$i.png" ; \
	done
	gcov src/graphviz/graphviz.c
	gcov src/util/util.c
	gcov src/util/queue.c
	gcov src/xml_builder/xml_builder.c
	gcov src/virtual/machine.c
	gcov src/abstract/ast.c
	rm *.gcov log



page: all
	echo "\n\n\033[32mGENERATING HTML\033[0m"
	for i in $(PAGE); do \
		echo -n -e "$$i ... "; \
		eval "./bin/parser.out <$$i"; \
	done
	if [ ! -d "bin/page" ];then mkdir bin/page; fi
	if [ ! -d "bin/page/html" ];then mkdir bin/page/html; fi
	if [ ! -d "bin/page/graph" ];then mkdir bin/page/graph; fi
	mv *.html bin/page/html
	mv *.dot bin/page/graph
	for i in bin/page/graph/*.dot; do \
		echo -n -e "$$i ... wait"; \
		dot -Tpng $$i -o $$i.png; \
		echo -e "\033[32mDONE\033[0m result: $$i.png" ; \
	done


draw:all
	$(MAKE) -C $(LIBS) draw

clean:
	$(MAKE) -C $(LIBS) clean;\
	rm -rf *.db *.g* log *.dot *.html
	if [ -d "lib" ];then rm -rf lib; fi
	if [ -d "bin" ];then rm -rf bin; fi
	if [ -d "doc" ];then rm -rf doc; fi

doc:	
	if [ ! -d "doc" ];then mkdir doc; fi
	doxygen Doxyfile
	mv latex/ html/ doc/
