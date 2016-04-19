#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ast.h>

enum ast_type find_ast_type(char *name);
char* name_ast_type(enum ast_type id);
enum binop find_binop(char *name);
char* name_binop(enum binop id);

void draw(struct ast* a);
void draw_ast(FILE * fd, struct ast* a);
void draw_tree(FILE * fd, struct tree* t);
void draw_match(FILE * fd, struct match* m);
void draw_cond(FILE * fd, struct cond *c);
void draw_fun(FILE * fd, struct fun* f);
void draw_patterns(FILE * fd, struct patterns* p);
void draw_pattern(FILE * fd, struct pattern* p);
void draw_app(FILE * fd, struct app* a);
void draw_forest(FILE * fd, struct forest* f);
void draw_attributes(FILE * fd, struct attributes * a);

#endif

