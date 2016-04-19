%{
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <util_parser.h>
#include <tree.h>
#include <ast.h>
#include <graphviz.h>

int yylex(void);
void yyerror(char *);
int yyparse(void);

struct ast* head = NULL;
void add_head(struct ast* node);
struct attributes* mk_attributes(struct ast* key, struct ast* value, struct attributes* next);
struct ast* add_right(struct ast* frst, struct ast* element);
struct ast* mk_comp_ast(char * str);


%}
%union{
    struct ast* node;
    struct attributes* attr;
    char* str;
};
                        
%token T_LABEL T_TEXT T_LET T_NB T_EMIT T_WHERE T_FUN T_ARROW T_IF T_THEN T_ELSE T_MATCH T_WITH T_END_MATCH T_COMP T_BINARY T_REC T_IN T_FILTER T_FILTER_SPACE T_VAR T_END_ATTRIBUT T_ATTRIBUT T_ERROR

%type   <attr>           assign
%type   <node>          T_TEXT let block_where block_in condition binary negation comparaison op_plus_moins op_mult_div expression variable begin_tree loop  container content op op2 declare var  args loop_text T_NB attribute emit
%type   <str>           T_LABEL T_BINARY T_COMP T_VAR T_ATTRIBUT T_EMIT
                        
%%

start:          start let end { add_head($let); }
        |       /* empty */
                ;


 //declaration global




let:            T_LET recursive declare ';'                 {printf("exp0\n");}
        |       block_where                                 {$$ = $block_where;}
        ;


block_where:    block_in  T_WHERE declare      {printf("where\n");}
        |       block_in                       {$$ = $block_in;}
                ;



block_in:       T_LET recursive declare T_IN block_in               {printf("block_in\n");}
        |       T_FUN args T_ARROW block_in                         
        |       condition                                           {$$ = $condition;}
                ;


 //condition
condition:      T_IF condition T_THEN condition T_ELSE condition    {$$ = mk_cond($2,$4,$6);}
        |       binary                                              {$$ = $binary;}
        ;


// operation binare
binary:         binary T_BINARY negation
                {
                  struct ast * app = mk_app(mk_comp_ast($T_BINARY), $1);
                  $$ = mk_app(app, $negation);
                }
        |       negation                                          {$$ = $negation;}
                ;

negation:       '!' negation
                {
                  $$ = mk_app(mk_unaryop(NOT), $2);
                }              
        |       comparaison                               {$$ = $comparaison;}
                ;

// operation comparaison
comparaison:    comparaison T_COMP op_plus_moins
                {
                  struct ast * app = mk_app(mk_comp_ast($T_COMP), $1);
                  $$ = mk_app(app, $op_plus_moins);
                }
        |       op_plus_moins                        {$$ = $op_plus_moins;}
                ;


// operation normal x
op_plus_moins:  op_plus_moins op op_mult_div
                {
                  struct ast * app = mk_app($op, $1);
                  $$ = mk_app(app, $op_mult_div);
                }
        |       op_mult_div                   {$$ = $op_mult_div;}
        ;


op_mult_div:
                op_mult_div op2 expression
                {
                  struct ast * app = mk_app($op2, $1);
                  $$ = mk_app(app, $expression);
                }
        |       expression                         {$$ = $expression;}
                ;

// variable et appel de fonction
expression:
               expression  variable
                {
                    add_right($1, $variable);
                    $$ = $1;
                }
        |       variable               {$$ = mk_forest(false,$variable,NULL);}
        ;


variable:      
                var                  {$$ = $var;}
        |       begin_tree           {$$ = $begin_tree;}
        ;



 //debut arbre
begin_tree:     
                attribute            {printf("begin_tree\n");}
        |       match                {$$ = mk_var("MATCH");}
        |       '(' loop ')'         {$$ = $loop;}
        |       T_NB                 {$$ = $T_NB;}
        |       emit                 {$$ = $emit;}
        |       container            {$$ = $container;}
        |       T_ERROR              {yyerror("invalid xml syntax");return EXIT_FAILURE;}
        ;


loop:
                loop block_where
                 {
                   if ($1 == NULL){
                     $$ = mk_forest(false,$block_where, NULL);
                }else{
                     add_right($1, $block_where);
                     $$ = $1;
                }
                }
        |       /* empty */          {$$ = NULL;}
                ;


attribute:
                T_LABEL '[' assign  T_END_ATTRIBUT      {$$ = mk_tree($T_LABEL,false,false,false,$assign,NULL);}
        |       T_LABEL '[' assign ']'  container       {$$ = mk_tree($T_LABEL,false,false,false,$assign,$container);}
        |       T_LABEL  container                      {$$ = mk_tree($T_LABEL,false,false,false,NULL,$container);}
        |       T_LABEL '/'                             {$$ = mk_tree($T_LABEL,false,true,false,NULL,NULL);}
                ;


// container
container:      '{' content '}'      {$$ = $content;}
                ;


content:        content block_where
                {
                  if ($1 == NULL){
                    $$ = mk_forest(false,$block_where, NULL);;
                    }else{
                    add_right($1, $block_where);
                    $$ = $1;
                  }
                }
        |       content T_TEXT
               {
                  
                  if ($1 == NULL)
                    $$ = mk_forest(false,$T_TEXT, NULL);
                  else{
                    add_right($1,$T_TEXT);
                    $$ = $1;   
                }
                
                }
        |       content ','          {$$ = $1;}
        |       /* empty */          {$$ = NULL;}
                ;


emit:           T_EMIT T_TEXT variable        { struct ast * body = mk_forest(false,$T_TEXT, $variable); $$ = mk_forest(false,mk_fun($T_EMIT,body),NULL); } 
        |       T_EMIT variable variable      { add_right($2, $3); $$ = mk_forest(false,mk_fun($T_EMIT,$2),NULL); }  
        ;
end:            ';'
        |       /* empty */
                ;


op:             '+'       {$$ = mk_binop(PLUS);}
        |       '-'       {$$ = mk_binop(MINUS);}
                ;


op2:            '*'       {$$ = mk_binop(MULT);}
        |       '/'       {$$ = mk_binop(DIV);}
        ;


declare:        args '=' block_where       {printf("je passe dans le Declare\n");}
        ;

var:            T_VAR                     {$$ =  mk_forest(false,mk_var($T_VAR),NULL);}
        |       T_ATTRIBUT                {$$ =  mk_forest(false,mk_var($T_ATTRIBUT),NULL);}
        ;

assign:         T_ATTRIBUT '=' loop_text assign    { $$ = mk_attributes(mk_var($T_ATTRIBUT),$loop_text, $4); }
        |       T_ATTRIBUT '=' loop_text           { $$ = mk_attributes(mk_var($T_ATTRIBUT),$loop_text, NULL); }
        ;


loop_text:      loop_text T_TEXT    {add_right($1,$T_TEXT); $$ = $1;}
        |       T_TEXT              {$$ = mk_forest(false,$T_TEXT,NULL);}
        ;


args:           args var            {add_right($1,$var); $$ = $1;}
        |       var                 {$$ = mk_forest(false,$var, NULL);}
        ;

recursive:      T_REC
        |       /*empty */
                ;
/* ===============================
          MATCH
   =============================== */


match:          T_MATCH block_where T_WITH extended_filter  T_END_MATCH
        ;


extended_filter:
                extended_filter '|' filter T_ARROW  recursive_match  {printf("extf1\n");}
        |       '|' filter T_ARROW recursive_match                   {printf("extf2\n");}
                ;


recursive_match:
                recursive_match block_where
        |       block_where
        ;


filter:         entete '{' filter_content '}'
        ;


filter_content:
                filter_content filter_contenu
        |       filter_content begin_tree
        |       /* empty */
        ;


entete:
                T_FILTER
        |       T_LABEL                 {printf("LABEL\n");}
        |       /* empty */
        ;

tag3:           T_FILTER
        |       var
        ;

tag4:           T_FILTER
        |       T_LABEL
        |       var
        ;

filter_contenu: 
                '*' tag3 '*'
        |       '/' tag4 '/'
        |       tag3
        ;



%%
               

void add_head(struct ast* node){
  struct forest* t = (struct forest *)head;

  if ( head == NULL){
    head = mk_forest(false, node, NULL);
  }
  else{
    while (t->tail != NULL){
      t = (struct forest*) t->tail;
    }
    struct ast* f = mk_forest(false, node, NULL);
    t->tail = f;
  }
}
        
                          
struct ast* mk_comp_ast(char * str){
  if(! strcmp(str, "==")){
    return mk_binop(EQ);
  }
  if(! strcmp(str, "<=")){
    return mk_binop(LEQ);
  }
  if(! strcmp(str, ">=")){
    return mk_binop(GEQ);
  }
  if(! strcmp(str, "<")){
    return mk_binop(LE);
  }
  if(! strcmp(str, ">")){
    return mk_binop(GE);
  }
  if(! strcmp(str, "||")){
    return mk_binop(OR);
  }
  if(! strcmp(str, "&&")){
    return mk_binop(AND);
  }
  return NULL;
}


struct ast* add_right(struct ast* frst, struct ast* element){
 if (frst == NULL)
    return mk_forest(false,element, NULL);

  struct forest* f = (struct forest*)frst;
  while (f->tail != NULL){
    f=(struct forest*)f->tail;
  }
  struct ast* new = mk_forest(false,element, NULL);
  f->tail = new;
  return frst;
}

struct attributes* mk_attributes(struct ast* key, struct ast* value, struct attributes* next){
  struct attributes * a = malloc(sizeof(*a));
  a->key = key;
  a->value = value;
  a->next = next;
  return a;
}


