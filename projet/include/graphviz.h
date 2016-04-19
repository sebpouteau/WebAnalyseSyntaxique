#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ast.h>

int cpt(void);

enum ast_type find_ast_type(char *name);
char* name_ast_type(enum ast_type id);
enum binop find_binop(char *name);
char* name_binop(enum binop id);

void draw(struct ast* a);
void draw_ast(FILE * fd, struct ast* a, char* name_parent);
void draw_tree(FILE * fd, struct tree* t, char* name_parent);
void draw_match(FILE * fd, struct match* m, char* name_parent);
void draw_cond(FILE * fd, struct cond *c, char* name_parent);
void draw_fun(FILE * fd, struct fun* f, char* name_parent);
void draw_patterns(FILE * fd, struct patterns* p, char* name_parent);
void draw_pattern(FILE * fd, struct pattern* p, char* name_parent);
void draw_app(FILE * fd, struct app* a, char* name_parent);
void draw_forest(FILE * fd, struct forest* f, char* name_parent);
void draw_attributes(FILE * fd, struct attributes * a, char* name_parent);

#endif

