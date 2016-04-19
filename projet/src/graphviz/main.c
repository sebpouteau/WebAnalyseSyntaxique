#include <graphviz.h>
#include <ast.h>
#include <stdio.h>
#include <stdlib.h>


int main(void){
  //struct ast *a = mk_integer(5);
  //struct ast *b = mk_binop(MINUS);
  //  struct ast *c = mk_forest(true, a, b);
  //struct ast *d = mk_var("a, b");
  struct ast *e = mk_word("a, b");

  printf("a\n");
  draw(e);
  return 0;
}
