#include "tree.h"
#include <assert.h>


struct attributes_t
{
  char* key;               //nom de l'attribut
  char* value;             //valeur de l'attribut
  attributes next; //attribut suivant
};

struct tree_t {
  char* label;     //étiquette du nœud
  bool nullary;     //nœud vide, par exemple <br/>
  bool space;       //nœud suivi d'un espace
  type tp;          //type du nœud. nullary doit être true si tp vaut word
  attributes attr;  //attributs du nœud
  tree daughters;   //fils gauche, qui doit être NULL si nullary est true
  tree right;       //frère droit
};

static int error(bool cond, char* message){
  if(!cond){
    fprintf(stderr, "/!\\ ERROR: %s\n", message);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}


tree create_empty_tree(){
  tree t = malloc(sizeof(*t));
  t->label = NULL;
  t->nullary = NULL;
  t->space = NULL;
  t->tp = _none;
  t->attr = NULL;
  t->daughters = NULL;
  t->right = NULL;
  return t;
}

tree create_tree(char* label, bool nullary, bool space, type tp, attributes attr, tree daughters, tree right){
  tree t = malloc(sizeof(*t));
  t->label = label;
  t->nullary = nullary;
  t->space = space;
  t->tp = tp;
  t->attr = attr;
  t->daughters = daughters;
  t->right = right;
  return t;
}

attributes create_empty_attributes(){
  attributes a = malloc(sizeof(*a));
  a->key = NULL;
  a->value = NULL;
  a->next = NULL;
  return a;
}

attributes create_attributes(char* key, char* value, attributes next){
  attributes a = malloc(sizeof(*a));
  a->key = key;
  a->value = value;
  a->next = next;
  return a;
}


/*========================================
  ======== GETTERS & SETTERS TREE ========
  ======================================== */


void set_label(tree t, char* label){
  t->label = label;
}

char* get_label(tree t){
  if(!error((t != NULL), "uninitialized tree")){
    if(!error((t->label !=NULL), "tree label isn't defined")){
      return t->label;
    }
  }
  return NULL;
}
/*
   assert(t != NULL && t->label != NULL){
     return t->label;
   }
}
*/

void set_nullary(tree t, bool nullary){
  t->nullary = nullary;
}

bool get_nullary(tree t){
  assert(t !=NULL);
  return t->nullary;
}

void set_space(tree t, bool space){
  t->space = space;
}

bool get_space(tree t){
  assert(t !=NULL);
  return t->space;
}


void set_tp(tree t, type tp){
  t->tp = tp;
}

type get_tp(tree t){
  assert(t != NULL && t->tp != _none);
  return t->tp;
}

void set_attributes(tree t, attributes attr){
  t->attr = attr;
}

attributes get_attributes(tree t){
  assert(t != NULL);
  return t->attr;
}

void set_daughters(tree t, tree d){
  t->daughters = d;
}

tree get_daughters(tree t){
  assert(t != NULL);
  return t->daughters;
}

void set_right(tree t, tree r){
  t->right = r;
}

tree get_right(tree t){
  assert(t != NULL);
  return t->right;
}


/*========================================
  ======== GETTERS & SETTERS ATTR ========
  ======================================== */


void set_key(attributes a, char* key){
  a->key = key;
}

char* get_key(attributes a){
  assert( a != NULL && a->key != NULL);
  return a->key;
}

void set_value(attributes a, char* value){
  a->value = value;
}

char* get_value(attributes a){
  assert(a != NULL && a->value != NULL);
  return a->value;
}

void set_next(attributes a, attributes next){
  a->next = next;
}

attributes get_next(attributes a){
  assert(a != NULL);
  return a->next;
}

