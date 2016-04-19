#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "graphviz.h"


void draw_ast (FILE * fd, struct ast* a){
  if (a == NULL){
    return;
  }
  
  switch(a->type){
    case INTEGER:
      printf ("INTEGER : %d\n", a->node->num);
      fprintf(fd, "%d;\n", a->node->num);
      break;
    case BINOP:
      printf ("BINOP : %d\n", a->node->binop);
      fprintf(fd, "%d;\n", a->node->binop);
      break;
    case UNARYOP:
      printf ("UNARYOP: %d\n", a->node->unaryop);
      fprintf(fd, "%d;\n", a->node->unaryop);
      break;
    case VAR:
      printf ("VAR\n");
      fprintf(fd, "\"%s\";\n", a->node->str);
      break;
    case IMPORT:
      printf ("IMPORT\n");
      // === a voir === //
      break;
    case APP:
      printf ("APP\n");
      fprintf(fd, "APP -> \n");
      draw_app(fd, a->node->app);
      break;
    case WORD:
      printf ("WORD\n");
      fprintf(fd, "\"%s\";\n", a->node->str);
      break;
    case TREE:
      printf ("TREE\n");
      fprintf(fd, "\"%s\" -> ", a->node->tree->label);
      draw_tree(fd, a->node->tree);
      break;
    case FOREST:
      printf ("FOREST\n");
      fprintf(fd, "FOREST -> ");
      draw_forest(fd, a->node->forest);
      break;
    case FUN:
      printf ("FUN\n");
      fprintf(fd, "FUN -> ");
      draw_fun(fd, a->node->fun);
      break;
    case MATCH:
      printf ("MATCH\n");
      fprintf(fd, "MATCH -> ");
      draw_match(fd, a->node->match);
      break;
    case COND:
      printf ("COND\n");
      fprintf(fd, "COND -> ");
      draw_cond(fd, a->node->cond);
      break;
    case DECLREC:
      printf ("DECLREC\n");
      fprintf(fd, "DECLREC -> ");
      draw_fun(fd, a->node->fun);
      break;
    default:
      break;
  }
}

void draw_app(FILE * fd, struct app* a){
  if (a == NULL){
    return;
  }
  
  draw_ast(fd, a->fun);
  draw_ast(fd, a->arg);
}

void draw_attributes(FILE * fd, struct attributes* a){
  if (a == NULL){
    return;
  }
  
  draw_ast(fd, a->key);
  draw_ast(fd, a->value);
  draw_attributes(fd, a->next);
}

void draw_tree(FILE * fd, struct tree* t){
  if (t == NULL){
    return;
  }
  
  printf("label: %s\n", t->label);
  printf("is_value: %d\n", t->is_value);
  printf("nullary: %d\n", t->nullary);
  printf("space: %d\n", t->space);
  draw_attributes(fd, t->attributes);
  draw_ast(fd, t->daughters);
}

void draw_forest(FILE * fd, struct forest* f){
  if (f == NULL){
    return;
  }
  
  printf("is_value: %d\n", f->is_value);
  draw_ast(fd, f->head);
  draw_ast(fd, f->tail);
}

void draw_fun(FILE * fd, struct fun* f){
  if (f == NULL){
    return;
  }
  
  printf("%s\n", f->id);
  draw_ast(fd, f->body);
}

void draw_patterns(FILE * fd, struct patterns* p){
  if (p == NULL){
    return;
  }
  
  draw_pattern(fd, p->pattern);
  draw_ast(fd, p->res);
  draw_patterns(fd, p->next);
}

void draw_match(FILE * fd, struct match* m){
  if (m == NULL){
    return;
  }
  
  draw_ast(fd, m->ast);
  draw_patterns(fd, m->patterns);
}

void draw_cond(FILE * fd, struct cond *c){
  if (c == NULL){
    return;
  }
  draw_ast(fd, c->cond);
  draw_ast(fd, c->then_br);
  draw_ast(fd, c->else_br);
}

void draw_pattern(FILE * fd, struct pattern* p){
  if (p == NULL){
    return;
  }
  //printf("pattern_type %s\n", p->ptype);
  //printf("pnode %s\n", p->pnode);
  // === a voir === //
}


void draw(struct ast* a){
  FILE *fd = fopen("fichier.dot", "w+");
  if (fd == NULL) {
    fprintf(stderr, "\"fichier.dot\": nom de fichier incorrect ou ouverture en ecriture impossible.");
    exit(EXIT_FAILURE);
  }
  
  fprintf(fd, "digraph{\n");
  draw_ast(fd, a);
  fprintf(fd, "}");
  
  fclose(fd);
}

int main(int argc, char *argv[]){
  struct ast *a = mk_integer(5);
  struct ast *b = mk_binop(MINUS);
  struct ast *c = mk_forest(true, a, b);
  //struct ast *d = mk_var("a, b");
  struct ast *e = mk_word("a, b");

  printf("a\n");
  draw(e);
  return 0;
}
