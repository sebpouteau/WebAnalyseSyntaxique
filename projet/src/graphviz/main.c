#include <graphviz.h>
#include <ast.h>
#include <stdio.h>
#include <stdlib.h>


int main(void){
  struct ast *a = mk_integer(5);
  printf("a\n");
  struct ast *d = mk_var("a");
  printf("b\n");
  struct ast *b = mk_forest(true, d, NULL);
  printf("c\n");
  struct ast *c = mk_forest(true, a, b);
  //struct ast *e = mk_word("a, b");


  printf("a\n");
  draw(c);
  printf("fin\n");
  return 0;
}
