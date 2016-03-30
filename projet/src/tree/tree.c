#include <tree.h>
#include <assert.h>

char *strdup(const char *);


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

static char* copy_string(char* s) {
  // char* new_s = malloc(sizeof(char) * strlen(s));
  return strdup(s);
  //return new_s;
}

tree tree_create_basic(char* label, type tp){
  return tree_create(copy_string(label),false,false,tp,NULL,NULL,NULL);
}

tree tree_create_empty(){
  return tree_create(NULL,NULL,NULL,_tree,NULL,NULL,NULL);
}


tree tree_create(char* label, bool nullary, bool space, type tp, attributes attr, tree daughters, tree right){
  tree t = malloc(sizeof(*t));
  t->label = copy_string(label);
  t->nullary = nullary;
  t->space = space;
  t->tp = tp;
  t->attr = attr;
  t->daughters = daughters;
  t->right = right;
  return t;
}

void tree_destroy(tree t) {
  if (t == NULL)
    return;
  
  free(t->label);

  tree_destroy(t->daughters);
  tree_destroy(t->right);
  attr_destroy(t->attr);

  free(t);
}
  

attributes attr_create_empty(){
  attributes a = malloc(sizeof(*a));
  a->key = NULL;
  a->value = NULL;
  a->next = NULL;
  return a;
}

attributes attr_create(char* key, char* value, attributes next){
  attributes a = malloc(sizeof(*a));
  a->key = copy_string(key);
  a->value = copy_string(value);
  a->next = next;
  return a;
}

void attr_destroy(attributes a) {
  if (a == NULL)
    return;
  free(a->key);
  free(a->value);
  attr_destroy(a->next);
  free(a);
}


/*========================================
  ======== GETTERS & SETTERS TREE ========
  ======================================== */

void tree_set_label(tree t, char* label){
  if (t == NULL)
    return;
  free(t->label);
  t->label = copy_string(label);
}

char* tree_get_label(tree t){
  assert(t != NULL);
  return t->label;
}

void tree_set_nullary(tree t, bool nullary){
  if (t == NULL)
    return;
  t->nullary = nullary;
}

bool tree_get_nullary(tree t){
  assert(t !=NULL);
  return t->nullary;
}

void tree_set_space(tree t, bool space){
  if (t == NULL)
    return;
  t->space = space;
}

bool tree_get_space(tree t){
  assert(t !=NULL);
  return t->space;
}


void tree_set_tp(tree t, type tp){
  if (t == NULL)
    return;
  t->tp = tp;
}

type tree_get_tp(tree t){
  assert(t != NULL);
  return t->tp;
}

// TODO Assigne à t une nouvelle pile d'attributs. Il faut donc supprimer l'ancienne et faire une copie profonde de attr
void tree_set_attributes(tree t, attributes attr){
  //attr_destroy(t->attributes);
  t->attr = attr;
}

attributes tree_get_attributes(tree t){
  assert(t != NULL);
  return t->attr;
}

void tree_set_daughters(tree t, tree d){
  if (t == NULL)
    return;
  t->daughters = d;
}

tree tree_get_daughters(tree t){
  assert(t != NULL);
  return t->daughters;
}

// TODO
void tree_set_right(tree t, tree r){
  t->right = r;
}

tree tree_get_right(tree t){
  assert(t != NULL);
  return t->right;
}

// TODO fonctions add_daugther qui ajoute un fils en parcourant la liste du fils gauche ?

/*========================================
  ======== GETTERS & SETTERS ATTR ========
  ======================================== */


void attr_set_key(attributes a, char* key){
  free(a->key);
  a->key = copy_string(key);
}

char* attr_get_key(attributes a){
  assert( a != NULL && a->key != NULL);
  return a->key;
}

void attr_set_value(attributes a, char* value){
  free(a->value);
  a->value = copy_string(value);
}

char* attr_get_value(attributes a){
  assert(a != NULL && a->value != NULL);
  return a->value;
}

// TODO
void attr_set_next(attributes a, attributes next){
  a->next = next;
}

attributes attr_get_next(attributes a){
  assert(a != NULL);
  return a->next;
}

void tree_add_daugthers(tree t, tree s){
  if (!tree_get_daughters(t)){
    tree_set_daughters(t,s);
    return;
  }
  t = tree_get_daughters(t);
  tree_add_right(t,s);
}


void tree_add_right(tree t, tree s){
  while (tree_get_right(t)){
    t = tree_get_right(t);
  }
  tree_set_right(t,s);
}


static void tree_draw_indent(int cpt) {
  for (int i = 0 ; i < cpt * 3 ; i++) {
    printf("-");
  }
}

static void tree_draw_attributes(attributes a, int cpt) {
  if (a == NULL)
    return;
  tree_draw_indent(cpt);

  printf("%s=%s\n", a->key, a->value);

  tree_draw_attributes(a->next, cpt);
}

static void tree_draw_r(tree t, int cpt) {
  if (t == NULL)
    return;

  tree_draw_indent(cpt);
  
  printf("%s\n", t->label);
  if (t->attr != NULL) {
    tree_draw_indent(cpt + 1);
    printf("Attributes :\n");
    tree_draw_attributes(t->attr, cpt + 2);
  }
  tree_draw_r(t->daughters, cpt + 1);
  tree_draw_r(t->right, cpt);
}

void tree_draw(tree t) {
  tree_draw_r(t, 0);
}


static attributes attr_copy(attributes a){
  if (a == NULL){
    return NULL;
  }
  attributes next = attr_copy(a->next);
  
  attributes new_a = attr_create(attr_get_key(a), attr_get_value(a), next);
  
  return new_a;
}


tree tree_copy(tree t){
  if (t == NULL){
    return NULL;
  }
  
  attributes a = attr_copy(t->attr);
  tree daughters = tree_copy(t->daughters);
  tree right = tree_copy(t->right);

  tree copy_t = tree_create(tree_get_label(t), tree_get_nullary(t), tree_get_space(t), tree_get_tp(t), a, daughters, right);
  
  return copy_t;
}
