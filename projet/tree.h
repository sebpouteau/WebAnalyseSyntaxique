#ifndef TREE_H
#define TREE_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum type_t {
  _tree,
  _word,
  _none
} type;

typedef struct tree_t* tree;
typedef struct attributes_t* attributes;

tree create_tree(char* label, bool nullary, bool space, type tp, attributes attr, tree daughters, tree right);
tree create_basic_tree(char* label, type tp);
tree create_empty_tree(void);

void destroy_tree(tree t);

attributes create_empty_attributes(void);
attributes create_attributes(char* key, char* value, attributes a);

void destroy_attributes(attributes a);

void set_label(tree t, char* label);
char* get_label(tree t);
void set_nullary(tree t, bool nullary);
bool get_nullary(tree t);
void set_space(tree t, bool space);
bool get_space(tree t);
void set_tp(tree t, type tp);
type get_tp(tree t);
void set_attributes(tree t, attributes attr);
attributes get_attributes(tree t);
void set_daughters(tree t, tree d);
tree get_daughters(tree t);
void set_right(tree t, tree r);
tree get_right(tree t);
void set_key(attributes a, char* key);
char* get_key(attributes a);
void set_value(attributes a, char* value);
char* get_value(attributes a);
void set_next(attributes a, attributes next);
attributes get_next(attributes a);

void draw(tree t);


#endif
