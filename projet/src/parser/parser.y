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
struct patterns* mk_patterns(struct  pattern * pattern, struct ast * res, struct patterns * next);
void add_patterns_right(struct patterns* filter, struct patterns* element);
void add_pforest_right(struct pattern* filter, struct pattern* element);

int compteur = 0;

%}
%union{
    struct ast* node;
    struct attributes* attr;
    struct pattern* pat;
    struct patterns* pats;
    char* str;
};
                        
%token T_LABEL T_TEXT T_LET T_NB T_EMIT T_WHERE T_FUN T_ARROW T_IF T_THEN T_ELSE T_MATCH T_WITH T_END_MATCH T_COMP T_BINARY T_REC T_IN T_FILTER T_FILTER_SPACE T_VAR T_END_ATTRIBUT T_ATTRIBUT T_ERROR T_FILTER_ACC PATH

%type   <attr>           assign
%type   <node>          T_TEXT let block_where block_in condition binary negation comparaison op_plus_moins op_mult_div expression  begin_tree loop  container content op op2 declare var  args loop_text T_NB attribute emit func match PATH
%type   <str>           T_LABEL T_BINARY T_COMP T_VAR T_ATTRIBUT T_EMIT entete T_FILTER T_FILTER_ACC tag3 tag4
%type   <pat>           filter_contenu filter_content filter

%type   <pats>          extended_filter                       
%%

start:          start let ';' { add_head($let); draw(head); }
                  |       start begin_tree { add_head($begin_tree); draw(head); }
        |       /* empty */
                ;



let:            T_LET recursive declare                  {printf("exp0\n");}
        |       block_where                                 {$$ = $block_where;}
        ;


block_where:    block_in  T_WHERE declare      {printf("where\n");}
        |       block_in                       {$$ = $block_in;}
                ;



block_in:       T_LET recursive declare T_IN block_in               {printf("block_in\n");}
        |       T_FUN args T_ARROW block_in                         {$$ =$4;}
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
                func begin_tree                  {$$ = $2;}
        |       func var                          {$$ =$1;}
        |       var                              {$$ =$1;}
                ;




 //debut arbre
begin_tree:     
                attribute            {printf("begin_tree\n");}
        |       match                {$$ = $match;}
        |       '(' loop ')'         {$$ = $loop;}
        |       T_NB                 {$$ = $T_NB;}
        |       emit                 {$$ = $emit;}
        |       container            {$$ = $container;}
        |       PATH                 {$$ = $PATH;}
        |       T_ERROR              {yyerror("invalid xml syntax");return EXIT_FAILURE;}
        ;


loop:
                 block_where
                 {
                   if ($1 == NULL){
                     $$ = mk_forest(false,$block_where, NULL);
                     compteur++;
                }else{
                     add_right($1, $block_where);
                     $$ = $1;
                     compteur++;
                 }
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
        |       '{' content func '}'      {$$ = $content;}
                ;


content:        content func ','
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
                  compteur++;
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



emit:           T_EMIT T_TEXT begin_tree        { struct ast * body = mk_forest(false,$T_TEXT, $3); $$ = mk_forest(false,mk_fun($T_EMIT,body),NULL); } 
        |       T_EMIT begin_tree begin_tree      { add_right($2, $3); $$ = mk_forest(false,mk_fun($T_EMIT,$2),NULL); }  
        ;


op:             '+'       {$$ = mk_binop(PLUS);compteur++;}
        |       '-'       {$$ = mk_binop(MINUS);compteur++;}
                ;


op2:            '*'       {$$ = mk_binop(MULT);compteur++;}
        |       '/'       {$$ = mk_binop(DIV);compteur++;}
        ;


declare:        args '=' block_where       {printf("je passe dans le Declare\n");}
        ;

var:            T_VAR                     {$$ =  mk_forest(false,mk_var($T_VAR),NULL);compteur++;}
        |       T_ATTRIBUT                {$$ =  mk_forest(false,mk_var($T_ATTRIBUT),NULL);compteur++;}
        ;

assign:         T_ATTRIBUT '=' loop_text assign    { $$ = mk_attributes(mk_var($T_ATTRIBUT),$loop_text, $4); compteur++;}
        |       T_ATTRIBUT '=' loop_text           { $$ = mk_attributes(mk_var($T_ATTRIBUT),$loop_text, NULL); compteur++;}
        ;


loop_text:      loop_text T_TEXT    {add_right($1,$T_TEXT); $$ = $1;compteur++;}
        |       T_TEXT              {$$ = mk_forest(false,$T_TEXT,NULL);compteur++;}
        ;


args:           args var            {add_right($1,$var); $$ = $1;compteur++;}
        |       var                 {$$ = $var;compteur++;}
        ;

recursive:      T_REC
        |       /*empty */
                ;
/* ===============================
          MATCH
   =============================== */


match:          T_MATCH block_where T_WITH extended_filter T_END_MATCH {$$ =  mk_match($block_where, $extended_filter);}
        ;


extended_filter:

                extended_filter '|' filter T_ARROW  block_where  { struct patterns* p = mk_patterns($filter, $block_where, NULL); add_patterns_right($1,p); $$ = $1;}
        |       '|' filter T_ARROW block_where                 { $$ = mk_patterns($filter, $4, NULL); }
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
                      $$ = mk_ptree(NULL, true, NULL);
                    else
                      $$ = $filter_content;
                }
                
               }  
        ;


filter_content:
                filter_content filter_contenu
                {
                  if ($1 == NULL)
                    $$ = mk_pforest($1 , NULL);
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
                  else
                    $$ = mk_pattern_var($tag3, TREEVAR);
                }
        |       filter {$$ = $1;}
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

void add_pforest_right(struct pattern* filter, struct pattern* element){
  struct pforest* forest = (struct pforest*) filter;
  while (forest->tail != NULL){
    forest = (struct pforest*) forest->tail;
  }
  struct pattern* f = mk_pforest(element, NULL);
  forest->tail = f;
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


struct patterns* mk_patterns(struct  pattern * pattern, struct ast * res, struct patterns * next)   {
  struct patterns *pat = malloc(sizeof(struct patterns));
  pat->next = next;
  pat->res = res;
  pat->pattern = pattern;
  return pat;
}

