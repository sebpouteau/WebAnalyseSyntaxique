%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "xml_builder.h"
int yylex(void);
void yyerror(char *);
int yyparse(void);

char* strdup(const char*); //TODO : Inutile a discuter avec Paul/Frantisek
int dup(int);
int dup2(int,int);
int close(int);
void add_head(tree t);
tree tree_create_text(char * text, tree right_tree);
tree evaluate(tree t); //substitution valeurs
tree evaluate_r(tree t);
tree head_tree = NULL;

// VARIABLES
typedef struct variable_t* variable;
struct variable_t{
    tree value;
    char *name;
    variable next;
};

variable first;

void var_init(void);
void var_quit(void);

variable var_create(char* name, tree t, variable next);
void var_destroy(variable v);

// Appelle récursivement var_destroy sur toutes les variables de la liste
void var_destroy_all(variable v);

void  var_assign(char * name, tree value);
tree var_get(char* name);

static variable var_lookup(char* name);
// FIN VARIABLES

void create_file(char *name,tree t);
%}

%union{
  char* value;
  struct tree_t* tree_parser;
  struct attributes_t*  attributes_parser;
 };

%type   <attributes_parser>  assign
%type   <tree_parser>        container content attributes begin affectation keywords
%token  <value> LABEL TEXT NAME LET EMIT IN WHERE AFFECT END_LET VAR VIRGULE

%%

start:          start begin '\n'
                {
                    add_head($2);
                    tree_draw(head_tree);
                    depth_search(head_tree,0);
                    tree_destroy(head_tree);
                    head_tree=NULL;
                    printf("\n");
                }
        |       start EMIT TEXT begin '\n'
                {
                  create_file($3,$4);
                }
        |	    start LET affectation END_LET '\n'
                {
                  printf("var save\n");
                  var_assign(tree_get_label(tree_get_daughters($affectation)), tree_get_right(tree_get_daughters($affectation)));
                }
        |       start keywords '\n'
        |       start '\n'
        |       start error '\n'
                {
                  yyerrok;
                }
        |       /* empty */
                ;

keywords:       LET affectation IN begin
                {
                  printf("coucou je suis ici j'ai bien trouvé ton keyword\n");

                  tree_set_right($affectation, $begin);
                  $$ = tree_create("in", false, false, _in, NULL, $affectation, NULL);
                  tree_draw($$);
                }
        |       begin WHERE affectation
                {
                  tree_set_right($affectation, $begin);
                  tree t = tree_create("where", false, false, _where, NULL, $affectation, NULL);
                  tree_draw(t);
                  $$ = evaluate(t);
                  tree_draw($$);
                }
        ;

affectation:	LABEL AFFECT begin
		{
                  printf("coucou je suis ici j'ai bien trouvé ta variable\n");
                  tree name = tree_create($LABEL, false, false, _word, NULL, NULL, $begin);
                  $$ = tree_create("=", false, false, _affect, NULL, name, NULL);
                  var_assign($1,$3);
                  //printf("sauvegarde variable\n");
		}
	;



begin:          LABEL '/'
                {
                  $$ = tree_create($1, true, false, _tree, NULL, NULL, NULL);

                  //printf("state: begin | format: LABEL /\n");
                }
        |       LABEL container
                {
                  $$ = tree_create($1, false, false, _tree, NULL, $2, NULL);
                  
                  //printf("state: begin | format: LABEL container\n");
                }
        |       attributes
                {
                  $$ = $1;
                  //printf("state: begin | format: attribute\n");
                }
        |       container
                {
                  $$ = $1;
                  //printf("state: begin | format: container\n");
                }
        ;


container:     '{' content '}'
                {
                  $$ = $2;
                }
        |       '{' content LABEL '}'
                {
                  tree t =  tree_create($LABEL, false, false, _var, NULL, NULL, NULL);
                  if (!$content)
                    $$ = t;
                  else{
                    tree_add_right($content,t);
                    $$ = $content;
                }
                }
                ;

content:        content TEXT 
                {
                    tree t = tree_create($2, true, true, _word, NULL,NULL, NULL);
                    if (!$1)
                      $$ = t;
                    else{
                      tree_add_right($1,t);
                      $$ = $1;
                      
                }
                    // printf("state: content | format: TEXT + content\n");
                }
        |       content attributes 
                {
                  if (!$1)
                    $$ = $2;
                  else{
                    tree_add_right($1,$2);
                    $$ = $1;
                  }
                  //printf("state: content | format: attribute + content\n");
                }
        |       content LABEL '/' 
                {
                  tree t = tree_create($2, true, false, _tree, NULL, NULL, NULL);
                  if (!$1)
                    $$ = t;
                  else {
                    tree_add_right($1,t);
                    $$ = $1;
                }
        //      printf("state: content | format: LABEL /\n");
                }
        |       content LABEL container 
                {
                  tree t = tree_create($2, false, false, _tree, NULL, $3, NULL);
                  if (!$1){
                    $$ = t;
                  }else{
                    tree_add_right($1,t);
                    $$ =$1;
                }
                  // printf("state: content | format: LABEL container + content\n");
                }
        |       content container 
                {
                  if (!$1)
                    $$=$2;
                  else {
                    tree_add_right($1,$2);
                    $$ = $1;
                  }
                  // printf("state: content | format: container\n");
                }
        |       content LABEL VIRGULE 
                {
                  tree t =  tree_create($2, false, false, _var, NULL, NULL, $1);
                  if (!$1)
                    $$ = t;
                  else{
                    tree_add_right($1,t);
                    $$=$1;
                    }
                }
        |       /* empty */
                {
                  $$ = NULL;
                }
                ;


attributes:      LABEL '[' assign ']' '/'
                {
                  $$ = tree_create($1, false, false, _tree, $3, NULL, NULL);
                  // printf("state: attribute | format: LABEL[assign]/\n");
                }
        |       LABEL '[' assign ']' separator container
                {
                  $$ = tree_create($1, false, false, _tree, $3, $6, NULL);
                  // printf("state: attribute | format: LABEL[assign] + separator + container\n");
                }
                ;

separator:
                separator ' '
        |       separator '\n'
        |       separator '\t'
        |       /* empty */
                ;



assign:         LABEL '=' TEXT assign
                {
                    $$ = attr_create($1, $3, $4);
                }
        |       LABEL '=' TEXT
                {
                    $$ =  attr_create($1, $3, NULL);
                }
        ;


%%
                /*

tree tree_create_text(char* text, tree right_tree){
  char *tmp;
  tmp=strtok(text," ");
  tree first = tree_create_empty();
  if (tmp){
    tree_set_tp(first,_word);
    tree_set_label(first,strdup(tmp));
    tree_set_nullary(first,true);
    tree_set_space(first,true);
    //first = tree_create(strdup(tmp),true,true,_word,NULL,NULL,NULL);
  }
  tree seconde = first;
  while (tmp){
    tree tmp_tree = tree_create(strdup(tmp),true,true,_word,NULL,NULL,NULL);
    tree_set_right(seconde, tmp_tree);
    seconde = tmp_tree;
    tmp=strtok(NULL," ");
  }
  tree_set_right(seconde, right_tree);
  return first;
}
                */

void add_head(tree t){
  if (head_tree == NULL){
    head_tree  = t;
    return;
  }
  tree_add_right(head_tree, t);
}
/*
tree evaluate(tree t){
  tree son = tree_get_daughters(t);
  while(son != NULL){
    if(tree_get_tp(son) == _var){
      //On récup l'arbre de la variable
      variable var = var_lookup(tree_get_label(son));
      //On remplace le fils actuel de notre arbre, par l'arbre de variable
      tree_set_daughters(var->value, tree_get_daughters(son));
      tree_set_right(var->value, tree_get_right(son));
      tree_destroy(son);
      tree_set_daughters(t, var->value);
      son = var->value;
    }
    else {
      son = tree_get_right(son);
    }
  }
  evaluate(tree_get_right(t));
  evaluate(tree_get_daughters(t));
  return t;
}
*/
/*
//Etienne version a tester
// TODO ne pas oublier de destroy les arbres après utilisation
tree evaluate_r (tree t, tree father) {
  if (t == NULL)
    return NULL;

  // Le premier fils est il une var ?
  while (tree_get_tp(t) == _var) { // Si on a remplacvé par une var faut aller voir
    tree_draw(var_get(tree_get_label(t)));
    
    tree cp = tree_copy(var_get(tree_get_label(t)));

    if (father != NULL)
      tree_set_daughters(father, cp);

    tree_add_right(cp, tree_get_right(t));
    tree_destroy(t); // On détruit le noeud var
    t = cp;
  }
  tree_set_daughters(t, evaluate_r(tree_get_daughters(t), t));
  
  // Y a t'il une var dans la suite de rights ?
  tree left = t;
  for ( tree act = tree_get_right(t) ; act != NULL ; act = tree_get_right(act)) {
    while (tree_get_tp(act) == _var) {
      tree cp = tree_copy(var_get(tree_get_label(act)));
      tree_set_right(left, cp);
      tree_add_right(cp, tree_get_right(act));
      tree_destroy(act);
    }
    tree_set_daughters(act, evaluate_r(tree_get_daughters(act), act));
  }

  return t;
}

tree evaluate(tree t) {
  if (t == NULL)
    return NULL;
  



tree evaluate(tree t) {
  return evaluate_r(t, NULL);
}

*/
tree evaluate(tree t){
  if (!t)
    return NULL;

  if(strcmp(tree_get_label(t),"=") == 0){
    tree label = tree_get_daughters(t);
    tree value = tree_get_right(label);
    var_assign(label,value);
  }

  if (tree_get_tp(t) == _var){
    tree cp = tree_copy(var_get(tree_get_label(t)));
    tree_set_daughters(t,cp);
  }
  
  evaluate(tree_get_right(t));
  evaluate(tree_get_daughters(t));
return t;
}


void var_init(void) {}

void var_quit(void) {
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

variable var_lookup(char* name){
  variable p;
  for (p = first; p != NULL && strcmp(p->name, name) != 0;p = p->next){}
  return p;
}

tree var_get(char* name) {
  variable p = var_lookup(name);
  assert(p != NULL);
  return p->value;
}

void var_assign(char* name, tree value){
  variable tmp = var_lookup(name);
  if (tmp == NULL)
    first = var_create (name, value, first);
  else
    tmp->value = value;
}

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
