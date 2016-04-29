%{
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ast.h>
#include <graphviz.h>
#include <queue.h>
#include <import.h>
#include <util.h>

int yylex(void);
void yyerror(char *);
int yyparse(void);


struct ast* head = NULL;
struct env * e = NULL;

%}
%union{
    struct ast* node;
    struct attributes* attr;
    struct pattern* pat;
    struct patterns* pats;
    struct queue_t* file;
    struct ast* ast;
    struct declare* decl;
    bool boolean;
    char* str;
};
                        
%token T_LABEL T_TEXT T_LET T_NB T_EMIT T_WHERE T_FUN T_ARROW T_IF T_THEN T_ELSE T_MATCH T_WITH T_END_MATCH T_COMP T_BINARY T_REC T_IN T_FILTER T_FILTER_SPACE T_VAR T_END_ATTRIBUT T_ATTRIBUT T_ERROR T_FILTER_ACC PATH

                        
%type   <attr>           assign
%type   <node>          T_TEXT  block_where block_in condition binary negation comparaison op_plus_moins op_mult_div expression  begin_tree  container content op op2  loop_text T_NB attribute emit function match PATH assignation
%type   <str>           T_LABEL T_BINARY T_COMP T_VAR T_ATTRIBUT T_EMIT entete T_FILTER T_FILTER_ACC tag3 tag4 var
%type   <pat>           filter_contenu filter_content filter
%type   <pats>          extended_filter
%type   <boolean>       recursive
%type   <decl>          declare
%type   <file>          args

                        
%%



/* ===============================
             GRAMMAIRE 
   =============================== */

// Etat initial 
start:          start let ';'           {}
        |       start block_where ';'   { add_head(head, $block_where, e);}
        |       start begin_tree        { add_head(head, $begin_tree, e);}
        |       start PATH ';'          {process_instruction($2,e);} 
        |       /*empty */
                ;


// Traitement des variable global
let:            T_LET recursive declare 
                 {
                   char* name = queue_front($declare->args);
                   queue_pop($declare->args);
                   struct ast* args = create_args($declare, name, $recursive);
                   e = process_binding_instruction(name , args, e);
                 }
        ;


// Traitement des block where
block_where:    block_in  T_WHERE recursive declare      {$$ = create_ast_in($1, $declare, $recursive);}
        |       block_in                                 {$$ = $block_in;}
                ;


// Traitement des IN mais aussi des fonctions anonyme
block_in:       T_LET recursive declare T_IN block_in               { $$ = create_ast_in($5, $declare, $recursive);}
        |       T_FUN args T_ARROW block_in
                {
                  struct declare* d = mk_declare($args,$4);
                  $$ = create_args(d,NULL,false);
                }
        |       condition                                           {$$ = $condition;}
                ;


// Traitement des conditions
condition:      T_IF condition T_THEN condition T_ELSE condition    {$$ = mk_cond($2,$4,$6);}
        |       binary                                              {$$ = $binary;}
        ;


// Traitement des operations binaires &&, ||
binary:         binary T_BINARY negation
                {
                  struct ast * app = mk_app(mk_comp_ast($T_BINARY), $1);
                  $$ = mk_app(app, $negation);
                }
        |       negation                              {$$ = $negation;}
                ;

// Traitement des negation "!"
negation:       '!' negation
                {
                  $$ = mk_app(mk_unaryop(NOT), $2);
                }              
        |       comparaison                           {$$ = $comparaison;}
                ;


// Traitement des comparaisons
comparaison:    comparaison T_COMP op_plus_moins
                {
                  struct ast * app = mk_app(mk_comp_ast($T_COMP), $1);
                  $$ = mk_app(app, $op_plus_moins);
                }
        |       op_plus_moins                        {$$ = $op_plus_moins;}
                ;


// Traitement des operations plus ou moins
op_plus_moins:  op_plus_moins op op_mult_div
                {
                  struct ast * app = mk_app($op, $1);
                  $$ = mk_app(app, $op_mult_div);
                }
        |       op_mult_div                   {$$ = $op_mult_div;}
        ;


// Traitement des operations multiplications et divisions
op_mult_div:
                op_mult_div op2 expression
                {
                  struct ast * app = mk_app($op2, $1);
                  $$ = mk_app(app, $expression);
                }
        |       expression                         {$$ = $expression;}
                ;


// variable et appel de fonction
expression:     function                     { $$ = $1; }
        |       begin_tree               { $$ = $1; }
        ;



// Traitement des fonctions
function:       function begin_tree                  {$$ = mk_app($1,$begin_tree);}
        |       function var                         {$$ = mk_app($1,mk_var($var));}
        |       var                                  {$$ = mk_var($1);}
                ;


// Début des types primitifs
begin_tree:     attribute              {$$ = $attribute;}
        |       match                  {$$ = $match;}
        |       '('block_where ')'     {$$ = mk_forest(false,$block_where, NULL);}
        |       T_NB                   {$$ = $T_NB;}
        |       emit                   {process_instruction($1,e);$$ = NULL ;} 
        |       container              {$$ = $container;}
        |       T_ERROR                {yyerror("invalid xml syntax");return EXIT_FAILURE;}
        ;



// Traitement des crochets
attribute:      T_LABEL '[' assign  T_END_ATTRIBUT      {$$ = mk_tree($T_LABEL,false,false,false,$assign,NULL); }
        |       T_LABEL '[' assign ']'  container       {$$ = mk_tree($T_LABEL,false,false,false,$assign,$container);}
        |       T_LABEL  container                      {$$ = mk_tree($T_LABEL,false,false,false,NULL,$container);}
        |       T_LABEL '/'                             {$$ = mk_tree($T_LABEL,false,true,false,NULL,NULL);}
                ;


// Traitement des accolades
container:      '{' content '}'      {$$ = $content;}
        |       '{' content function '}'
                {
                  if ($2 == NULL){
                    $$ = mk_forest(false, $function, NULL);
                  }
                else{
                  add_right($2,$function);
                  $$ = $2;
                    
                }
                }
                ;

// Traitement des contenu des accolades
content:        content function ','
                {
                  if ($1 == NULL){
                    $$ = mk_forest(false, $function, NULL);
                } else{
                  add_right($1,$function);
                  $$ = $1;
                }
                }
        |       content begin_tree
                {
                  if ($1 == NULL){
                    $$ = mk_forest(false,$begin_tree, NULL);;
                  }else{
                    add_right($1, $begin_tree);
                    $$ = $1;
                  }
                }
        |       content T_TEXT
               {
                 if ($1 == NULL){
                   $$ = mk_forest(false,$T_TEXT, NULL);
                } else{
                   add_right($1,$T_TEXT);
                   $$ = $1;
                }
                }
        |       /*empty */          {$$ = NULL;}
                ;


/* ===============================
              UTILS
   =============================== */



// fonction emit, pour ecrire dans un fichier
emit:           T_EMIT T_TEXT begin_tree        { struct ast * em = mk_app(mk_binop(EMIT),$T_TEXT->node->tree->daughters); $$ = mk_app(em,mk_forest(false,$begin_tree, NULL)); } 
        ;


// opération plus et moins
op:             '+'       {$$ = mk_binop(PLUS);}
        |       '-'       {$$ = mk_binop(MINUS);}
                ;


// Opération multiplication division
op2:            '*'       {$$ = mk_binop(MULT);}
        |       '/'       {$$ = mk_binop(DIV);}
        ;


//  suite d argument et l affectation
declare:        args '=' block_where       {struct declare* d = mk_declare($args, $block_where); $$ = d;}
        ;

// Variable
var:            T_VAR                     {$$ =  $T_VAR;}
        |       T_ATTRIBUT                {$$ =  $T_ATTRIBUT;}
        ;

// Assignation dans les crochets
assign:         T_ATTRIBUT '=' assignation assign    { $$ = mk_attributes(mk_word($T_ATTRIBUT),$assignation, $4); }
        |       T_ATTRIBUT '=' assignation           { $$ = mk_attributes(mk_word($T_ATTRIBUT),$assignation, NULL); }
        ;

// Boucle pour le texte dans assign
assignation:    loop_text {$$=$1;}
        |       T_NB        {$$ = $1;}
        ;

loop_text:      loop_text T_TEXT    {add_right($1,$T_TEXT); $$ = $1;}
        |       T_TEXT              {$$ = mk_forest(true,$T_TEXT,NULL);}
        ;

// suite de variable pour les declarions
args:           args var            {queue_push($1, $var); $$ = $1;}
        |       var                 {struct queue_t * q = mk_queue(); queue_push(q,$var); $$ = q;}
        ;

// Fonction recursive ou non
recursive:      T_REC      {$$ = true;}
        |       /*empty */ {$$ = false;}
                ;



/* ===============================
          MATCH
   =============================== */

// Traitement du match
match:          T_MATCH block_where T_WITH extended_filter T_END_MATCH    {$$ =  mk_match($block_where, $extended_filter);}
        ;


// Liste des filtres
extended_filter:
                extended_filter '|' filter T_ARROW  block_where
                {
                  struct patterns* p = mk_patterns($filter, $block_where, NULL);
                  add_patterns_right($1,p);
                  $$ = $1;
                }
        |       '|' filter T_ARROW block_where                   { $$ = mk_patterns($filter, $block_where, NULL); }
                ;


// Traitement d'un filtre
filter:         entete '{' filter_content '}'
                {
                  if($entete!=NULL){
                    if (! strcmp($entete, "_"))
                      if ($filter_content == NULL)
                        $$ = mk_anytree(true, NULL);
                      else {
                        $$ = mk_anytree(false, $filter_content);
                      }
                  
                  else{
                    if ($filter_content == NULL)
                      $$ = mk_ptree($entete, true, NULL);
                    else
                      $$ = mk_ptree($entete, false, $filter_content);
                 }
                }else {
                    if ($filter_content == NULL)
                      $$ = NULL;
                    else
                      $$ = $filter_content;
                }
                
               }  
        ;

// Contenu d'un filtre
filter_content:
                filter_content filter_contenu
                {
                  if ($1 == NULL){
                    $$ = mk_pforest($2 , NULL);
                    }
                  else {
                    add_pforest_right($1, $2);
                    $$ = $1;
                }
                }
        |       /* empty */ {$$ = NULL;}
        ;


// Contenu entre accolades dans les filtres
filter_contenu: 
                  '*' tag3 '*'
                {
                  if (! strcmp($tag3,"_"))
                    $$ = mk_wildcard(ANYSTRING);
                  else
                    $$ = mk_pattern_var($tag3, STRINGVAR);
                }
        |       '/' tag4 '/'
                {
                  if (! strcmp($tag4, "_"))
                    $$ = mk_wildcard(ANYFOREST);
                  else
                    $$ = mk_pattern_var($tag4, FORESTVAR);
                } 
        |       tag3
                 {
                  if (! strcmp($tag3, "_"))
                    $$ = mk_wildcard(ANY);
                  else{
                    $$ = mk_pattern_var($tag3, TREEVAR);
                    }
                }
        |       T_TEXT  {$$ = mk_pstring($1->node->tree->daughters->node->str);} 
        |       filter {$$ = $1;}
        ;


// Entete d'un filtre
entete:
                T_FILTER_ACC  {$$ =$1;}
        |       T_LABEL       {$$ =$1;}         
        |       /* empty */   {$$ = NULL;}
        ;


tag3:           T_FILTER      {$$ =$1;}
        |       T_ATTRIBUT     {$$ =$1;}
        |       T_VAR          {$$ =$1;}
        ;

tag4:           T_FILTER       {$$ =$1;}
        |       T_LABEL         {$$ =$1;}
        |       T_ATTRIBUT        {$$ =$1;}
        |       T_VAR            {$$ =$1;}
        ;
%%
               


