#include <tree.h>
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

// TODO error ou assert il faut choisir
// return exitfailure wtf exit(exit_failure) ou return true false

static int error(bool cond, char* message){
  if(!cond){
    fprintf(stderr, "/!\\ ERROR: %s\n", message);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static char* copy_string(char* s) {
  char* new_s = malloc(sizeof(char) * strlen(s));
  strcpy(new_s, s);
  return new_s;
}

tree create_basic_tree(char* label, type tp){
  return create_tree(copy_string(label),false,false,tp,NULL,NULL,NULL);
}

tree create_empty_tree(){
  return create_tree(NULL,NULL,NULL,_tree,NULL,NULL,NULL);
}


tree create_tree(char* label, bool nullary, bool space, type tp, attributes attr, tree daughters, tree right){
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

void destroy_tree(tree t) {
  if (t == NULL)
    return;
  
  free(t->label);

  destroy_tree(t->daughters);
  destroy_tree(t->right);

  destroy_attributes(t->attr);
  free(t);
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
  a->key = copy_string(key);
  a->value = copy_string(value);
  a->next = next;
  return a;
}

void destroy_attributes(attributes a) {
  if (a == NULL)
    return;
  free(a->key);
  free(a->value);
  destroy_attributes(a->next);
  free(a);
}


/*========================================
  ======== GETTERS & SETTERS TREE ========
  ======================================== */

void set_label(tree t, char* label){
  free(t->label);
  t->label = copy_string(label);
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
  //assert(t != NULL && t->tp != _none);
  assert(t != NULL);
  return t->tp;
}

// TODO Assigne à t une nouvelle pile d'attributs. Il faut donc supprimer l'ancienne et faire une copie profonde de attr
void set_attributes(tree t, attributes attr){
  //destroy_attributes(t->attributes);
  t->attr = attr;
}

attributes get_attributes(tree t){
  assert(t != NULL);
  return t->attr;
}

// TODO
void set_daughters(tree t, tree d){
  t->daughters = d;
}

tree get_daughters(tree t){
  assert(t != NULL);
  return t->daughters;
}

// TODO
void set_right(tree t, tree r){
  t->right = r;
}

tree get_right(tree t){
  assert(t != NULL);
  return t->right;
}

// TODO fonctions add_daugther qui ajoute un fils en parcourant la liste du fils gauche ?

/*========================================
  ======== GETTERS & SETTERS ATTR ========
  ======================================== */


void set_key(attributes a, char* key){
  free(a->key);
  a->key = copy_string(key);
}

char* get_key(attributes a){
  assert( a != NULL && a->key != NULL);
  return a->key;
}

void set_value(attributes a, char* value){
  free(a->value);
  a->value = copy_string(value);
}

char* get_value(attributes a){
  assert(a != NULL && a->value != NULL);
  return a->value;
}

// TODO
void set_next(attributes a, attributes next){
  a->next = next;
}

attributes get_next(attributes a){
  assert(a != NULL);
  return a->next;
}

void draw_indent(int cpt) {
  for (int i = 0 ; i < cpt * 3 ; i++) {
    printf("-");
  }
}

void draw_attributes(attributes a, int cpt) {
  if (a == NULL)
    return;
  draw_indent(cpt);

  printf("%s=%s\n", a->key, a->value);

  draw_attributes(a->next, cpt);
}

static void draw_r(tree t, int cpt) {
  if (t == NULL)
    return;

  draw_indent(cpt);
  
  printf("%s\n", t->label);
  if (t->attr != NULL) {
    draw_indent(cpt + 1);
    printf("Attributes :\n");
    draw_attributes(t->attr, cpt + 2);
  }
  draw_r(t->daughters, cpt + 1);
  draw_r(t->right, cpt);
}

void draw(tree t) {
  draw_r(t, 0);
}
