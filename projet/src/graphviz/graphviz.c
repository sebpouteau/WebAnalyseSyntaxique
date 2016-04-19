#include <stdio.h>
#include <stdlib.h>
#include <graphviz.h>

struct {
  char *name;
  int id;
} conv_ast_type[DECLREC+1] =
  {
    {"INTEGER", INTEGER},
    {"BINOP", BINOP},
    {"UNARYOP", UNARYOP},
    {"VAR", VAR},
    {"IMPORT", IMPORT},
    {"APP", APP},
    {"WORD", WORD},
    {"TREE", TREE},
    {"FOREST", FOREST},
    {"FUN", FUN},
    {"MATCH", MATCH},
    {"COND", COND},
    {"DECLREC", DECLREC}
  };


enum ast_type find_ast_type(char *name){
  for (int i = 0; i < DECLREC; i++)
    if (strcmp(name, conv_ast_type[i].name) == 0)
      return conv_ast_type[i].id;
  return DECLREC;
}


char *name_ast_type(enum ast_type id){
  if (id < DECLREC)
    return conv_ast_type[id].name;
  return conv_ast_type[DECLREC].name;
}


struct {
  char *name;
  int id;
} conv_binop[AND+1] =
  {
    {"PLUS", PLUS},
    {"MINUS", MINUS},
    {"MULT", MULT},
    {"DIV", DIV},
    {"LEQ", LEQ},
    {"LE", LE},
    {"GEQ", GEQ},
    {"GE", GE},
    {"EQ", EQ},
    {"OR", OR},
    {"AND", AND}
  };


enum binop find_binop(char *name){
  for (int i = 0; i < AND; i++)
    if (strcmp(name, conv_binop[i].name) == 0)
      return conv_binop[i].id;
  return AND;
}


char *name_binop(enum binop id){
  if (id < AND)
    return conv_ast_type[id].name;
  return conv_ast_type[AND].name;
}




int cpt(){
  static int cpt = -1;
  cpt++;
  return cpt;
}

void draw_ast (FILE * fd, struct ast* a){
  if (a == NULL){
    fprintf(fd, "NULL;\n");
  }
  
  switch(a->type){
    case INTEGER:
      printf ("\"INTEGER : %d\n", a->node->num);
      fprintf(fd, "\"%d (%d)\";\n", a->node->num, cpt());
      break;
    case BINOP:
      printf ("BINOP : %d\n", a->node->binop);
      fprintf(fd, "\"%d (%d)\";\n", a->node->binop, cpt());
      break;
    case UNARYOP:
      printf ("UNARYOP: %d\n", a->node->unaryop);
      fprintf(fd, "\"%d (%d)\";\n", a->node->unaryop, cpt());
      break;
    case VAR:
      printf ("VAR\n");
      fprintf(fd, "\"%s (%d)\";\n", a->node->str, cpt());
      break;
    case IMPORT:
      printf ("\"IMPORT (%d)\"\n", cpt());
      // === a voir === //
      break;
    case APP:
      printf ("APP\n");
      fprintf(fd, "\"APP (%d)\"-> \n", cpt());
      draw_app(fd, a->node->app);
      break;
    case WORD:
      printf ("WORD\n");
      fprintf(fd, "\"%s (%d)\";\n", a->node->str, cpt());
      break;
    case TREE:
      printf ("TREE\n");
      fprintf(fd, "\"%s (%d)\" -> ", a->node->tree->label, cpt());
      draw_tree(fd, a->node->tree);
      break;
    case FOREST:
      printf ("FOREST\n");
      fprintf(fd, "\"FOREST (%d)\" -> ", cpt());
      draw_forest(fd, a->node->forest);
      break;
    case FUN:
      printf ("FUN\n");
      fprintf(fd, "\"FUN (%d)\" -> ", cpt());
      draw_fun(fd, a->node->fun);
      break;
    case MATCH:
      printf ("MATCH\n");
      fprintf(fd, "\"MATCH (%d)\" -> ", cpt());
      draw_match(fd, a->node->match);
      break;
    case COND:
      printf ("COND\n");
      fprintf(fd, "\"COND (%d)\" -> ", cpt());
      draw_cond(fd, a->node->cond);
      break;
    case DECLREC:
      printf ("DECLREC\n");
      fprintf(fd, "\"DECLREC (%d)\" -> ", cpt());
      draw_fun(fd, a->node->fun);
      break;
    default:
      break;
  }
}

void draw_app(FILE * fd, struct app* a){
  if (a == NULL){
    fprintf(fd, "NULL;\n");
  }
  
  draw_ast(fd, a->fun);
  draw_ast(fd, a->arg);
}

void draw_attributes(FILE * fd, struct attributes* a){
  if (a == NULL){
    fprintf(fd, "NULL;\n");
  }
  
  draw_ast(fd, a->key);
  draw_ast(fd, a->value);
  draw_attributes(fd, a->next);
}

void draw_tree(FILE * fd, struct tree* t){
  if (t == NULL){
    fprintf(fd, "NULL;\n");
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
    fprintf(fd, "NULL;\n");
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
    fprintf(fd, "NULL;\n");
  }
  
  draw_ast(fd, m->ast);
  draw_patterns(fd, m->patterns);
}

void draw_cond(FILE * fd, struct cond *c){
  if (c == NULL){
    fprintf(fd, "NULL;\n");
  }
  draw_ast(fd, c->cond);
  draw_ast(fd, c->then_br);
  draw_ast(fd, c->else_br);
}

void draw_pattern(FILE * fd, struct pattern* p){
  if (p == NULL){
    fprintf(fd, "NULL;\n");
  }
  //printf("pattern_type %s\n", p->ptype);
  //printf("pnode %s\n", p->pnode);
  // === a voir === //
}


void draw(struct ast* a){
  FILE *fd = fopen("fichier.dot", "w+");
  if (fd == NULL) {
    fprintf(stderr, "\"fichier.dot\": erreur ouverture fichier.");
    exit(EXIT_FAILURE);
  }
  
  fprintf(fd, "digraph{\n");
  draw_ast(fd, a);
  fprintf(fd, "}");
  
  fclose(fd);
}
