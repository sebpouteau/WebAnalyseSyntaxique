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
  struct text_t* n_text;
  struct tree_t* tree_parser;
  struct attributes_t*  attributes_parser;
 };

%type   <attributes_parser>  assign
%type   <tree_parser>        container content attributes begin affectation keywords in_affectation argument
%token  <n_text>          TEXT
%token  <value> LABEL NAME LET EMIT IN WHERE AFFECT END_LET VAR VIRGULE FUN

                        
%%

start:          start begin '\n'
                {
                  add_head($2);
                  tree_draw(head_tree);
                  depth_search(head_tree,0);
                  tree_destroy(head_tree);
                  head_tree=NULL;
                  printf("\n");
                  var_quit(first);
                    
                }
        |       start EMIT TEXT begin '\n'
                {
                  create_file(text_get_word($TEXT),$4);
                  text_free($TEXT);
                }
        |       start LET affectation END_LET '\n'
                {
                  //                  var_assign(tree_get_label(tree_get_daughters($affectation)), tree_get_right(tree_get_daughters($affectation)),first);

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
                  $$ = tree_create("in", false, false, _local, NULL, $affectation, NULL);
                  //tree_draw($$);
                }
        |       begin WHERE affectation
                {
                  tree_set_right($affectation, $begin);
                  tree t = tree_create("where", false, false, _local, NULL, $affectation, NULL);
                  //tree_draw(t);
                  $$ = t;//evaluate(t);
                  printf("============================\n");
                  printf("=========TREE FINAL=========\n");
                  printf("============================\n");
                  tree_draw($$);
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

                printf("coucou je suis ici j'ai bien trouvé ta variable/ fonction\n");
                printf("type : %d\n", tree_get_tp(tete));
                tree_draw(tete);
                free($LABEL);
                $$ = tete;
                
                //var_assign(tete,first); // TODO refaire assign //
		}
                ;

in_affectation: begin
                {
                  $$ = tree_create("body", false, false, _body, NULL, $begin, NULL);
                }
        |       FUN argument '-' '>' begin
                {
                  tree body = tree_create("body", false, false, _body, NULL, $begin, NULL);
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

begin:          LABEL '/'
                {
                  $$ = tree_create($1, true, false, _tree, NULL, NULL, NULL);
                  free($LABEL);
                  //printf("state: begin | format: LABEL /\n");
                }
        |       LABEL container
                {
                  $$ = tree_create($1, false, false, _tree, NULL, $2, NULL);
                  free($LABEL);
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
                  printf("in content LABEL\n");
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
                 
        //      printf("state: content | format: TEXT + content\n");
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
                  free($LABEL);
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
                  free($LABEL);
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


attributes:      LABEL '[' assign ']' '/'
                {
                  $$ = tree_create($1, false, false, _tree, $3, NULL, NULL);
                  free($LABEL);
                  // printf("state: attribute | format: LABEL[assign]/\n");
                }
        |       LABEL '[' assign ']' separator container
                {
                  $$ = tree_create($1, false, false, _tree, $3, $6, NULL);
                  free($LABEL);
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


%%



void add_head(tree t){
  if (head_tree == NULL){
    head_tree  = t;
    return;
  }
  tree_add_right(head_tree, t);
}


tree evaluate(tree t)
{
  //t NULL -> quit
  if(t == NULL)
    return NULL;

  printf("label de l'arbre : %s\n", tree_get_label(t));
  
  printf("===============\n");
  tree_draw(t);

  if(tree_get_tp(t) == _variable)
  {
    printf("in_var where label = %s\n\n", tree_get_label(t));
    //ce tmp contient l'arbre décrivant la variable
    tree tmp = tree_copy(var_get(tree_get_label(t),first));
    //On se donne comme fils la valeur de la variable
    while(tree_get_tp(tmp) != _body){
      tmp=tree_get_right(tmp);
    }
    tree_set_daughters(t, tree_get_daughters(tmp));
  }

  else if(tree_get_tp(t) == _declaration)
  {
    //si c'est de type fun on créer une nouvelle "variable" contenant l'arbre décrivant la fonction
    var_assign(tree_get_label(t), tree_get_daughters(t), first);
    if (tree_get_daughters(tree_get_daughters(t)) != NULL){ //je possede des arguments -> je suis une fonction

    }
    
  }
  
  printf("===============\n");
  
  evaluate(tree_get_right(t));
  evaluate(tree_get_daughters(t));
  return t;
}

/*
tree evaluate_function(tree t){
  

}*/


