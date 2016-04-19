#ifndef UTIL_PARSER_H
#define UTIL_PARSER_H
#include <stdbool.h>
#include <tree.h>
#include <assert.h>

typedef struct text_t* text;

text text_create(char* ,bool);
void text_free(text);
bool text_get_space(text);
char* text_get_word(text);



typedef struct variable_t* variable;


void var_init(void);
void var_quit(variable);

variable var_create(char* name, tree t, variable next);

void var_destroy(variable v);

void var_destroy_all(variable v);

void  var_assign(char * name, tree value, variable);

tree var_get(char* name, variable);

variable var_lookup(char* name, variable);

void create_file(char *name,tree t);

#endif
