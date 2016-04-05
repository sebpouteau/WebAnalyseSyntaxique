#include <util_parser.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xml_builder.h>
#include <unistd.h>

/* ===============================
      strucuture pour les TEXT
   =============================== */

struct text_t{
  char* word;
  bool space;
};

text text_create(char* word, bool space){
  text t = malloc(sizeof(struct text_t));
  t->word = strdup(word);
  t->space = space;
  return t;
}

void text_free(text t){
  free(t->word);
  free(t);
}

char* text_get_word(text t){
  return strdup(t->word);
}

bool text_get_space(text t){
  return t->space;
}

/* ===============================
      strucuture pour les VAR
   =============================== */


struct variable_t{
    tree value;
    char *name;
    variable next;
};



//PEUT ETRE CREER UN FICHIER POUR TRAITER LES VARS ? RESSEMBLE A UNE INTERFACE
void var_init(void) {}

void var_quit(variable first) {
  var_destroy_all(first);
}


void var_destroy_all(variable v) {
  if (v == NULL)
    return;
  var_destroy_all(v->next);
  var_destroy(v);
}

void var_destroy(variable v) {
  free(v->name);
  free(v);
}

variable var_create(char* name, tree value, variable next){
    variable tmp =  malloc(sizeof(*tmp));
    tmp->name = strdup(name);
    tmp->value = value; // TODO COPY TREE ?
    tmp->next = next;

    return tmp;
}

variable var_lookup(char* name,variable first){
  variable p;
  for (p = first; p != NULL && strcmp(p->name, name) != 0;p = p->next){}
  return p;
}

tree var_get(char* name, variable first) {
  variable p = var_lookup(name, first);
  assert(p != NULL);
  return p->value;
}

void var_assign(char* name, tree value, variable first){
  variable tmp = var_lookup(name, first);
  if (tmp == NULL)
    first = var_create (name, value, first);
  else
    tmp->value = value;
}



/* ===============================
      CREATION FICHIER EMIT
   =============================== */


void create_file(char *name,tree t){
  printf("\n");
  int fd = open(name,O_CREAT|O_TRUNC|O_RDWR,0700);
  int save = dup(1);
  dup2(fd,1);
  depth_search(t,0);
  dup2(save,1);
  close(fd);
  close(save);
}
