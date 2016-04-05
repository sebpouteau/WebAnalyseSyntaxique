#ifndef TREE_H
#define TREE_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum type_t {
  _tree,
  _word,
  _body,
  _local,
  _affect,
  _arg,
  
  _variable,
  _function,
  _declaration,

  _none
} type;

typedef struct tree_t* tree;
typedef struct attributes_t* attributes;

tree tree_create(char* label, bool nullary, bool space, type tp, attributes attr, tree daughters, tree right);
tree tree_create_basic(char* label, type tp);
tree tree_create_empty(void);

void tree_destroy(tree t);

void tree_set_label(tree t, char* label);
char* tree_get_label(tree t);

void tree_set_nullary(tree t, bool nullary);
bool tree_get_nullary(tree t);

void tree_set_space(tree t, bool space);
bool tree_get_space(tree t);

void tree_set_tp(tree t, type tp);
type tree_get_tp(tree t);

void tree_set_attributes(tree t, attributes attr);
attributes tree_get_attributes(tree t);

void tree_set_daughters(tree t, tree d);
tree tree_get_daughters(tree t);

void tree_set_right(tree t, tree r);
tree tree_get_right(tree t);

void tree_add_daugthers(tree t, tree s);
void tree_add_right(tree t, tree s);

void tree_draw(tree t);

attributes attr_create_empty(void);
attributes attr_create(char* key, char* value, attributes a);

void attr_destroy(attributes a);

void attr_set_key(attributes a, char* key);
char* attr_get_key(attributes a);

void attr_set_value(attributes a, char* value);
char* attr_get_value(attributes a);

void attr_set_next(attributes a, attributes next);
attributes attr_get_next(attributes a);

tree tree_copy(tree t);

#endif

