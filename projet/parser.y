%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"

int yylex(void);
void yyerror(char *);
int yyparse(void);


%}

%union{
  char * value;
 };
%token LABEL
%token TEXT
                        
%%

start:          start begin '\n'
        |       start '\n'
        |       start error
        |       /* empty */ 
                ;

begin:          LABEL '/'         {printf("state: begin | format: LABEL /\n");}
        |       LABEL container   {printf("state: begin | format: LABEL container\n");}
        |       attribute         {printf("state: begin | format: attribute\n");}
        |       container         {printf("state: begin | format: container\n");}
        ;

container:     '{' content '}'
                ;

content:        TEXT content              {printf("state: content | format: TEXT + content\n");}
        |       attribute content         {printf("state: content | format: attribute + content\n");}
        |       LABEL '/'                 {printf("state: content | format: LABEL /\n");}
        |       LABEL container content   {printf("state: content | format: LABEL container + content\n");}
        |       container                 {printf("state: content | format: container\n");}
        |       /*empty */                {printf("state: content | format: empty content\n");}
                ;


attribute:      LABEL '[' assign ']' '/'                     {printf("state: attribute | format: LABEL[assign]/\n");}
        |       LABEL '[' assign ']' separator container     {printf("state: attribute | format: LABEL[assign] + separator + container\n");}
                ;

separator:       
                separator ' '
        |       separator '\n'
        |       separator '\t'
        |       /* empty */
                ;



assign:         LABEL '=' TEXT assign
        |       LABEL '=' TEXT
        ;





%%


