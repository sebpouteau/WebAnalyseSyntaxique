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


void add_head(tree t);

tree create_tree_text(char * text, tree right_tree);
tree head_tree = NULL;
int numberTree = 0;


struct variable{
    tree value;
    char *name;
    struct variable *next;
    };

struct variable *first, *last;

struct variable* new_var(char* name);
void  assign(char * name, tree value);
struct variable* lookup(char* name);

%}

%union{
  char* value;
  struct tree_t* tree_parser;
  struct attributes_t*  attributes_parser;
 };

%type   <attributes_parser>  assign
%type   <tree_parser>        container content attributes begin
%token  <value> LABEL TEXT NAME LET


%%

start:          start begin '\n'
                {
		    add_head($2);
                    draw(head_tree);
                    depth_search(head_tree,0);
		    destroy_tree(head_tree);
		    head_tree=NULL;
		    printf("\n");
                    }
	|	start variable '\n'
        |       start '\n'
        |       start error
        |       /* empty */
                ;

variable:	LET NAME '=' begin ';'
		{
		    assign($2,$4);
		    printf("sauvegarde variable\n");
		}
	;
begin:          LABEL '/'
                {
                  $$ = create_tree($1, true, false, _tree, NULL, NULL, NULL);

                  //printf("state: begin | format: LABEL /\n");
                }
        |       LABEL container
                {
                  $$ = create_tree($1, false, false, _tree, NULL, $2, NULL);
                  
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
                  $$ = create_tree($1, false, false, _tree, $3, NULL, NULL);
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




void add_head(tree t){
  if (head_tree == NULL){
    head_tree  = t;
    return;
  }
  add_right(head_tree, t);
}


struct variable* new_var(char* name){
    struct variable* tmp =  malloc(sizeof(struct variable));
    tmp->name = strdup(name);
    tmp->value = 0;
    tmp->next = NULL;
    if (first == NULL)
	first = last = tmp;
    else
       {
	   last->next = tmp;
	   last = tmp;
       }
    return tmp;
}

struct variable* lookup(char* name){
    for (struct variable* p = first; p != NULL ;p = p->next){
	if(strcmp(p->name, name) == 0)
	    return p;
    }
    return new_var(name);
}

void  assign(char* name, tree value){
  struct variable* tmp = lookup(name);
  tmp->value = value;
}
