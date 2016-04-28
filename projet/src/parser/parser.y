%{
#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <ast.h>
#include <graphviz.h>
#include <queue.h>
#include <import.h>

int yylex(void);
void yyerror(char *);
int yyparse(void);

struct declare{
  struct queue_t* args;
  struct ast* body;
};

struct ast* head = NULL;
void add_head(struct ast* node);

struct attributes* mk_attributes(struct ast* key, struct ast* value, struct attributes* next);
struct ast* add_right(struct ast* frst, struct ast* element);
struct ast* mk_comp_ast(char * str);
struct patterns* mk_patterns(struct  pattern * pattern, struct ast * res, struct patterns * next);

void add_patterns_right(struct patterns* filter, struct patterns* element);
void add_pforest_right(struct pattern* filter, struct pattern* element);

struct ast* create_args(struct declare* decl, char*name, bool is_rec);
struct ast* create_ast_in(struct ast* body, struct declare* decl, bool is_rec);

struct declare*  mk_declare(struct queue_t* q, struct ast* a);

struct env * e = NULL;
int compteur = 0;
void print_env(struct env* e);

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
%type   <node>          T_TEXT  block_where block_in condition binary negation comparaison op_plus_moins op_mult_div expression  begin_tree loop  container content op op2  loop_text T_NB attribute emit func match PATH assignation
%type   <str>           T_LABEL T_BINARY T_COMP T_VAR T_ATTRIBUT T_EMIT entete T_FILTER T_FILTER_ACC tag3 tag4 var
%type   <pat>           filter_contenu filter_content filter

%type   <pats>          extended_filter
%type   <boolean>       recursive
%type   <decl>          declare
%type   <file>          args
%%

start:          start let ';' {  }
                  |       start block_where ';' { if ($block_where != NULL){struct closure* c = process_content($block_where, e);add_head(c->value);draw(head);} }
        |       start begin_tree      { if ($begin_tree != NULL){struct closure* c = process_content($begin_tree, e);add_head(c->value);draw(head);} }
        |       /*empty */
                ;



let:            T_LET recursive declare 
                 {
                   char* name = queue_front($declare->args);
                   queue_pop($declare->args);
                   struct ast* args = create_args($declare, name, $recursive);
                   e = process_binding_instruction(name , args, e);
                   printf("\n name function: %s\n", name);
                   //print_env(e);
                }
        ;


block_where:    block_in  T_WHERE recursive declare      {$$ = create_ast_in($1, $declare, $recursive);}
        |       block_in                       {$$ = $block_in;}
                ;


block_in:       T_LET recursive declare T_IN block_in               { $$ = create_ast_in($5, $declare, $recursive);}
        |       T_FUN args T_ARROW block_in
                {
                  struct declare* d = mk_declare($args,$4);
                  $$ = create_args(d,NULL,false);
                }
        |       condition                                           {$$ = $condition;}
                ;



  
 //condition
condition:      T_IF condition T_THEN condition T_ELSE condition    {$$ = mk_cond($2,$4,$6); compteur++;}
        |       binary                                              {$$ = $binary;}
        ;


// operation binare
binary:         binary T_BINARY negation
                {
                  struct ast * app = mk_app(mk_comp_ast($T_BINARY), $1);
                  $$ = mk_app(app, $negation);
                  compteur++;
                }
        |       negation                                          {$$ = $negation;}
                ;

negation:       '!' negation
                {
                  $$ = mk_app(mk_unaryop(NOT), $2);
                  compteur++;
                }              
        |       comparaison                               {$$ = $comparaison;}
                ;


// operation comparaison
comparaison:    comparaison T_COMP op_plus_moins
                {
                  struct ast * app = mk_app(mk_comp_ast($T_COMP), $1);
                  $$ = mk_app(app, $op_plus_moins);
                  compteur++;
                }
        |       op_plus_moins                        {$$ = $op_plus_moins;}
                ;


// operation normal x
op_plus_moins:  op_plus_moins op op_mult_div
                {
                  struct ast * app = mk_app($op, $1);
                  $$ = mk_app(app, $op_mult_div);
                  compteur++;
                }
        |       op_mult_div                   {$$ = $op_mult_div;}
        ;


op_mult_div:
                op_mult_div op2 expression
                {
                  struct ast * app = mk_app($op2, $1);
                  $$ = mk_app(app, $expression);
                  compteur++;
                }
        |       expression                         {$$ = $expression;}
                ;

// variable et appel de fonction
expression:
               func
                {
                    $$ = $1;
                    compteur++;
                }
        |       begin_tree               {$$ = $1;}
        ;




func:      
                func begin_tree                  {$$ = mk_app($1,$begin_tree);}
        |       func var                         {$$ = mk_app($1,mk_var($var));}
        |       var                              {$$ = mk_var($1);}
                ;




 //debut arbre
begin_tree:     
                attribute            {printf("begin_tree\n");}
        |       match                {$$ = $match;}
        |       '(' loop ')'         {$$ = $loop;}
        |       T_NB                 {$$ = $T_NB;}
|       emit                 {process_instruction($1,e);$$ = NULL ;} 
        |       container            {$$ = $container;}
        |       PATH                 {$$ = $PATH;}
        |       T_ERROR              {yyerror("invalid xml syntax");return EXIT_FAILURE;}
        ;


loop:
                 block_where
                 {
                     $$ = mk_forest(false,$block_where, NULL);
              
                }
        |       /* empty */          {$$ = NULL;}
                ;


attribute:
                T_LABEL '[' assign  T_END_ATTRIBUT      {$$ = mk_tree($T_LABEL,false,false,false,$assign,NULL); compteur++;}
        |       T_LABEL '[' assign ']'  container       {$$ = mk_tree($T_LABEL,false,false,false,$assign,$container);compteur++;}
        |       T_LABEL  container                      {$$ = mk_tree($T_LABEL,false,false,false,NULL,$container);compteur++;}
        |       T_LABEL '/'                             {$$ = mk_tree($T_LABEL,false,true,false,NULL,NULL);compteur++;}
                ;


// container
container:      '{' content '}'      {$$ = $content;}
        |       '{' content func '}'
                {
                  if ($2 == NULL){
                    $$ = mk_forest(false, $func, NULL);
                  }
                else{
                  add_right($2,$func);
                  $$ = $2;
                    
                }
                }
                ;


content:        content func ','
                {
                  if ($1 == NULL){
                    $$ = mk_forest(false, $func, NULL);
                    
                }
                else{
                  add_right($1,$func);
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
                    compteur++;
                  }
                }
        |       content T_TEXT
               {
                  if ($1 == NULL){
                    $$ = mk_forest(false,$T_TEXT, NULL);
                    
                  }
                  else{
                    add_right($1,$T_TEXT);
                    $$ = $1;
                    
                }
                }
        |       /* empty */          {$$ = NULL;}
                ;



emit:           T_EMIT T_TEXT begin_tree        { struct ast * emit = mk_app(mk_binop(EMIT),$T_TEXT->node->tree->daughters); $$ = mk_app(emit,$begin_tree); } 
        ;


op:             '+'       {$$ = mk_binop(PLUS);compteur++;}
        |       '-'       {$$ = mk_binop(MINUS);compteur++;}
                ;


op2:            '*'       {$$ = mk_binop(MULT);compteur++;}
        |       '/'       {$$ = mk_binop(DIV);compteur++;}
        ;


declare:        args '=' block_where       {struct declare* d = mk_declare($args, $block_where); $$ = d;}
        ;

var:            T_VAR                     {$$ =  $T_VAR;}
        |       T_ATTRIBUT                {$$ =  $T_ATTRIBUT;}
        ;

assign:         T_ATTRIBUT '=' assignation assign    { $$ = mk_attributes(mk_word($T_ATTRIBUT),$assignation, $4); compteur++;}
        |       T_ATTRIBUT '=' assignation           { $$ = mk_attributes(mk_word($T_ATTRIBUT),$assignation, NULL); compteur++;}
        ;

assignation:    loop_text {$$=$1;}
        |       T_NB        {$$ = $1;}
        ;
loop_text:      loop_text T_TEXT    {add_right($1,$T_TEXT); $$ = $1;compteur++;}
        |       T_TEXT              {$$ = mk_forest(true,$T_TEXT,NULL);compteur++;}
        ;


args:           args var            {queue_push($1, $var); $$ = $1;}
        |       var                 {struct queue_t * q = mk_queue(); queue_push(q,$var); $$ = q;}
        ;

recursive:      T_REC      {$$ = true;}
        |       /*empty */ {$$ = false;}
                ;

/* ===============================
          MATCH
   =============================== */


match:          T_MATCH block_where T_WITH extended_filter T_END_MATCH {$$ =  mk_match($block_where, $extended_filter);}
        ;


extended_filter:
                extended_filter '|' filter T_ARROW  block_where  { struct patterns* p = mk_patterns($filter, $block_where, NULL); add_patterns_right($1,p); $$ = $1;}
        |       '|' filter T_ARROW block_where                 { $$ = mk_patterns($filter, $block_where, NULL); }
                ;


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
                      $$ = NULL;// mk_ptree(NULL, true, NULL);
                    else
                      $$ = $filter_content;
                }
                
               }  
        ;


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
                    printf("je passe dans fi\n");
                    $$ = mk_pattern_var($tag3, TREEVAR);
                    }
                }
        |       T_TEXT  {$$ = mk_pstring($1->node->tree->daughters->node->str);} 
        |       filter {$$ = $1;}
        ;

%%
               

void add_head(struct ast* node){
  if ( head == NULL){
    head = mk_forest(false, node, NULL);
    return;
  }

  struct forest* t = head->node->forest;
  while (t->tail != NULL){
      t = (struct forest*) t->tail;
  }
  struct ast* f = mk_forest(false, node, NULL);
  t->tail = f; 
}

void add_pforest_right(struct pattern* filter, struct pattern* element){
  struct pforest* forest = filter->pnode->pforest;
  while (forest->tail != NULL){
    forest =  forest->tail->pnode->pforest;
  }
  if(element != NULL){
    struct pattern* f = mk_pforest(element, NULL);
    forest->tail = f;
  }
}

void add_patterns_right(struct patterns* filter, struct patterns* element){
  while (filter->next != NULL){
    filter =  filter->next;
  }
 
  filter->next = element;
}
  
                          
struct ast* mk_comp_ast(char * str){
  if(! strcmp(str, "==")){
    return mk_binop(EQ);
  }
  if(! strcmp(str, "!=")){
    return mk_binop(NEQ);
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

  struct forest* f = frst->node->forest;
  while (f->tail != NULL){
    f=f->tail->node->forest;
  }
  struct ast* new = mk_forest(false,element, NULL);
  f->tail = new; 
  return frst;
}

struct attributes* mk_attributes(struct ast* key, struct ast* value, struct attributes* next){
  struct attributes * a = malloc(sizeof(*a));
  a->is_value = true;
  a->key = key;
  a->value = value;
  a->next = next;
  return a;
}


struct patterns* mk_patterns(struct  pattern * pattern, struct ast * res, struct patterns * next)   {
  struct patterns *pat = malloc(sizeof(struct patterns));
  pat->next = next;
  pat->res = res;
  pat->pattern = pattern;
  return pat;
}

struct ast* create_ast_in(struct ast* body, struct declare* decl, bool is_rec){
  char* name = queue_front(decl->args);
  queue_pop(decl->args);
  
  struct ast* fun = mk_fun(name, body);
  struct ast* args = create_args(decl, name, is_rec);
 
  struct ast* a = mk_app(fun,args);
  a = mk_forest(false, a, NULL);
  return a;
}

struct ast* create_args(struct declare* decl, char*name, bool is_rec){
  if(queue_empty(decl->args)){
    return decl->body;
  }
  char * n =(char *) queue_front(decl->args);
  queue_pop(decl->args);
  struct ast* a = mk_fun(n , create_args(decl, n, false));
  if (is_rec)
    a = mk_declrec(name, a);
  return a;
}

struct declare*  mk_declare(struct queue_t* q, struct ast* a){
  struct declare * d = malloc(sizeof(*d));
  d->args = q;
  d->body = a;
  return d;
}


void print_env(struct env* en){
  while (en != NULL){
    printf("%s\n", en->var);
    en= en->next;
  }
}
