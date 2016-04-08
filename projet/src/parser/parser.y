%{
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <util_parser.h>
#include <tree.h>

int yylex(void);
void yyerror(char *);
int yyparse(void);

%}
                        
%token LABEL TEXT LET NB EMIT WHERE FUN ARROW IF THEN ELSE MATCH WITH END_MATCH COND BINARY REC EOL IN XML FILTER FILTER_SPACE VAR

%%

start:          start block  {printf(" ======FIN====== \n" );}
        |       /* empty */
        ;

block:          NB  //{printf("NB\n");}
        |       tag container //{printf("tag container\n");}
        |       LABEL '/' //{printf("label / \n");}
        |       LABEL '['  assign ']' '/' //{printf("label [ assign ]/\n");}
        |       LABEL '[' assign  ']' container  //{printf("label [ assign ]container\n");}
        |       LET  declaration ';'  //{printf("let declaration\n");}
        |       local_affect  //{printf("local_affect\n");}
        |       MATCH block WITH extended_filter  END_MATCH  //{printf("match\n");}
                ;


local_affect:   LET declaration  IN  block 
        |       block  WHERE  declaration
        ;

tag:            LABEL
        |       /* empty */
        ;


container:     '{' extended_content '}'
        |      '{' extended_content args '}'
                ;


extended_content:
                extended_content content
        |       /* empty */
        ;


content:        TEXT
        |       block
        |       args ','
        ;


assign:         VAR '=' TEXT assign
        |       VAR '=' TEXT 
                ;


declaration:    rec args space '=' block 
        |       rec args space '=' FUN  args  ARROW  block 
        ;

rec:            REC
        |       /* empty */
        ;

args:           args VAR
        |       '(' args VAR')'
        |       VAR
        ;



extended_filter:
                extended_filter space '|' filter ARROW block
        |       space '|' filter ARROW block
                ;


filter:         filter filter_contenu space
        |       filter_contenu
        ;


tag2:           FILTER
        |       LABEL
        ;

entete:
                tag2 
        |       /* empty */
        ;


filter_contenu: 
                '*' tag2 '*'
        |       '/' tag2 '/'
        |       filter_options
        ;

filter_options:
                entete '{' filter '}'
        |       FILTER_SPACE 
                ;


space:
                space ' '
        |       space EOL
        |       space '\t'
        |       /* empty */
                ;


%%
              
