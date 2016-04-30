#include <graphviz.h>
#include <ast.h>
#include <stdio.h>
#include <stdlib.h>


int main(void){
  /* struct ast *a = mk_integer(5);
  printf("a\n");
  struct ast *d = mk_var("a");
  printf("b\n");
  struct ast *b = mk_forest(true, d, NULL);
  printf("c\n");
  struct ast *c = mk_forest(true, a, b);
  //struct ast *e = mk_word("a, b");
  */
  struct ast* app1 = mk_app(mk_binop(MINUS), mk_var("x"));
  struct ast* app2 = mk_app(app1, mk_integer(1));
  printf("a\n");
  draw("test.html",app2);
  printf("fin\n");
  return 0;
}
