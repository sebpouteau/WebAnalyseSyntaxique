%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <tree.h>
#include <xml_builder.h>
#include <util_parser.h>

int yylex(void);
void yyerror(char *);
int yyparse(void);

tree tree_create_text(char * text, tree right_tree);
tree evaluate(tree t); //substitution valeurs
tree evaluate_r(tree t);
tree head_tree = NULL;

variable first;
void add_head(tree t);

%}

                        
%union{
  char* value;
  int number;
  struct text_t* n_text;
  struct tree_t* tree_parser;
  struct attributes_t*  attributes_parser;
 };

%type   <attributes_parser>  assign
%type   <tree_parser>        container content attributes node
                         affectation local_affect in_affectation argument
%type   <value>         tag
%token  <n_text>          TEXT
%token  <number>        NB
%token  <value> LABEL NAME LET EMIT IN WHERE AFFECT END_LET VAR VIRGULE FUN XML 
%token EOL IF THEN ELSE MATCH ARROW WITH
                        
%%

start:          start node EOL
                   {
                     add_head($2);
                     tree_draw(head_tree);
                     depth_search(head_tree,0);
                     tree_destroy(head_tree);
                     head_tree=NULL;
                     printf("\n");
                     var_quit(first);
                }
        |       start EMIT TEXT node EOL
        |       start LET affectation END_LET EOL
        |       start local_affect EOL
        |       start EOL
        |       start error EOL
        |       /* empty */
                ;



node:           LABEL '/'
                {
                  $$ = tree_create($LABEL, true, false, _tree, NULL, NULL, NULL);
                  free($LABEL);
                }
        |       tag container
                {
                  $$ = $container;
                  if ($tag != NULL){
                    $$ = tree_create($tag, false, false, _tree, NULL, $container, NULL);
                    free($tag);
                  }
                }
        |       attributes
                {
                  $$ = $attributes;
                }
        ;


container:     '{' content '}'
                {
                  $$ = $2;
                }
        |       '{' content LABEL '}'
                {
                  tree t =  tree_create($LABEL, false, false, _variable, NULL, NULL, NULL);
                  free($LABEL);
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
                  tree t = tree_create(text_get_word($TEXT), true, text_get_space($TEXT), _word, NULL,NULL, NULL);
                  free($TEXT);                      
                  if (!$1)
                    $$ = t;
                  else{
                    tree_add_right($1,t);
                    $$ = $1;   
                  }
                }
        |       content attributes 
                {
                  if (!$1)
                    $$ = $2;
                  else{
                    tree_add_right($1,$2);
                    $$ = $1;
                  }
                }
        |       content LABEL '/' 
                {
                  tree t = tree_create($2, true, false, _tree, NULL, NULL, NULL);
                  free($LABEL);
                  if (!$1)
                    $$ = t;
                  else {
                    tree_add_right($1,t);
                    $$ = $1;
                }
                }
        |       content tag container 
                {
                  tree t = $container;
                  if ($tag != NULL){
                    t = tree_create($tag, false, false, _tree, NULL, $container, NULL);
                    free($tag);
                  }
                  if (!$1){
                    $$ = t;
                  }else{
                    tree_add_right($1,t);
                    $$ =$1;
                  }
                }
        |       content LABEL VIRGULE 
                {
                  tree t =  tree_create($2, false, false, _variable, NULL, NULL, $1);
                  free($LABEL);
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

tag:         LABEL
                { $$ = $LABEL;}
        |       /* empty */
                {$$ = NULL;}
        ;



attributes:     LABEL '[' assign ']' '/'
                {
                  $$ = tree_create($1, false, false, _tree, $3, NULL, NULL);
                  free($LABEL);
                }
        |       LABEL '[' assign ']' separator container
                {
                  $$ = tree_create($1, false, false, _tree, $3, $6, NULL);
                  free($LABEL);

                }
                ;

separator:
                separator ' '
        |       separator EOL
        |       separator '\t'
        |       /* empty */
                ;



assign:         LABEL '=' TEXT assign
                {
                  $$ = attr_create($1, text_get_word($TEXT), $4);
                  free($TEXT);
                  free($LABEL);
                }
        |       LABEL '=' TEXT
                {
                  $$ =  attr_create($1, text_get_word($TEXT), NULL);
                  free($TEXT);
                  free($LABEL);
                }
        ;



local_affect:   LET affectation IN node
                {
                  tree_set_right($affectation, $node);
                  $$ = tree_create("in", false, false, _local, NULL, $affectation, NULL);
                }
        |       node WHERE affectation
                {
                  tree_set_right($affectation, $node);
                  tree t = tree_create("where", false, false, _local, NULL, $affectation, NULL);
                  $$ = t;
                }
        ;


affectation:	LABEL argument AFFECT in_affectation
		{
                  tree arg = tree_create("argument", false,false, _arg, NULL,NULL,NULL);
                  if ($argument != NULL){
                    tree_add_daugthers(arg,$argument);
                }
                tree_add_right(arg,$in_affectation);
                tree tete = tree_create($LABEL, false, false, _declaration, NULL, arg, NULL);
                tree_draw(tete);
                free($LABEL);
                $$ = tete;
                
		}
                ;

in_affectation: node
                {
                  $$ = tree_create("body", false, false, _body, NULL, $node, NULL);
                }
        |       FUN argument '-' '>' node
                {
                  tree body = tree_create("body", false, false, _body, NULL, $node, NULL);
                  tree_add_right($argument, body);
                  $$ = $argument;
                }
                ;

argument:       argument LABEL
                {
                  tree t = tree_create($LABEL, true, false, _word, NULL, NULL, NULL);
                  free($LABEL);
                  if ($1 == NULL)
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



%%



void add_head(tree t){
  if (head_tree == NULL){
    head_tree  = t;
    return;
  }
  tree_add_right(head_tree, t);
}
