%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "web.h"

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

begin:          LABEL '/'         {printf("LABEL /\n");}
        |       LABEL container   {printf("LABEL container\n");}
        |       attribute         {printf("Attribute\n");}
        |       container
                {
                  printf("container\n");}
                ;

container:     '{' content '}'
                ;

content:        TEXT content
        |       attribute content
        |       container
        |       /*empty */
                ;


attribute:      LABEL '[' assign ']' '/'
        |       LABEL '[' assign ']' separator container
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


