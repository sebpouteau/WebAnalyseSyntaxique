%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"
#include "xml_builder.h"
int yylex(void);
void yyerror(char *);
int yyparse(void);
char* strdup(char*);

void add_daugthers(tree t, tree s);
void add_right(tree t, tree s);
  void add_head(tree t);

tree create_tree_text(char * text, tree right_tree);
tree head_tree = NULL;
int numberTree = 0;
%}

%union{
  char* value;
  struct tree_t* tree_parser;
  struct attributes_t*  attributes_parser;
 };

%type   <attributes_parser>  assign
%type   <tree_parser>        container content attributes
%token  <value> LABEL TEXT


%%

start:          start begin '\n'
                {
                    draw(head_tree);
                    depth_search(head_tree,0);
                    head_tree=NULL;
                    }
        |       start '\n'
        |       start error
        |       /* empty */
                ;

begin:          LABEL '/'
                {
                  tree tmp = create_tree($1, true, false, _tree, NULL, NULL, NULL);
                  add_head(tmp);
                  //printf("state: begin | format: LABEL /\n");
                }
        |       LABEL container
                {
                  tree tmp = create_tree($1, false, false, _tree, NULL, $2, NULL);
                  add_head(tmp);
                  //printf("state: begin | format: LABEL container\n");
                }
        |       attributes
                {
                  add_head($1);
                  //printf("state: begin | format: attribute\n");
                }
        |       container
                {
                  add_head($1);
                  //printf("state: begin | format: container\n");
                }
        ;


container:     '{' content '}'
                {
                  $$ = $2;
                }
                ;

content:        TEXT content
                {
                    $$= create_tree($1, true, true, _word, NULL,NULL, $2);
                    // printf("state: content | format: TEXT + content\n");
                }
        |       attributes content
                {
                  set_right($1,$2);
                  $$ = $1;
                  //printf("state: content | format: attribute + content\n");
                }
        |       LABEL '/' content
                {
                  $$ = create_tree($1, true, false, _tree, NULL, NULL, NULL);
                  //printf("state: content | format: LABEL /\n");
                }
        |       LABEL container content
                {
                  add_right($2,$3);
                  $$ = create_tree($1, false, false, _tree, NULL, $2, NULL);
                  // printf("state: content | format: LABEL container + content\n");
                }
        |       container
                {
                  $$=$1;
                  // printf("state: content | format: container\n");
                }
        |       /* empty */
                {
                  $$ = NULL;
                }
                ;


attributes:      LABEL '[' assign ']' '/'
                {
                  $$ = create_tree($1, true, false, _tree, $3, NULL, NULL);
                  // printf("state: attribute | format: LABEL[assign]/\n");
                }
        |       LABEL '[' assign ']' separator container
                {
                  $$ = create_tree($1, false, false, _tree, $3, $6, NULL);
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
                    $$ = create_attributes($1, $3, $4);
                }
        |       LABEL '=' TEXT
                {
                    $$ =  create_attributes($1, $3, NULL);
                }
        ;


%%
                /*

tree create_tree_text(char* text, tree right_tree){
  char *tmp;
  tmp=strtok(text," ");
  tree first = create_empty_tree();
  if (tmp){
    set_tp(first,_word);
    set_label(first,strdup(tmp));
    set_nullary(first,true);
    set_space(first,true);
    //first = create_tree(strdup(tmp),true,true,_word,NULL,NULL,NULL);
  }
  tree seconde = first;
  while (tmp){
    tree tmp_tree = create_tree(strdup(tmp),true,true,_word,NULL,NULL,NULL);
    set_right(seconde, tmp_tree);
    seconde = tmp_tree;
    tmp=strtok(NULL," ");
  }
  set_right(seconde, right_tree);
  return first;
}
                */



void add_daugthers(tree t, tree s){
  if (!get_daughters(t)){
    set_daughters(t,s);
    return;
  }
  t = get_daughters(t);
  add_right(t,s);
}


void add_right(tree t, tree s){
  while (get_right(t)){
    t = get_right(t);
  }
  set_right(t,s);
}

void add_head(tree t){
  if (head_tree == NULL){
    head_tree  = t;
    return;
  }
  add_right(head_tree, t);
}
