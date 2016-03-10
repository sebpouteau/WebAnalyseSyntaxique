%{
#include <string.h>
#include <stdio.h>
#include "parser.tab.h"
char* strdup(char *);
int fileno(FILE *);
%}


label (_[[:alpha:]]|[[:alpha:]])[[:alnum:]\'._]*
xml (?i:xml)({label})*

%x TEXT_MODE
%s TREE_MODE

%option noyy_top_state
%option noinput nounput stack

%%



{xml} {}

{label}   {
  yylval.value = strdup(yytext);
  return LABEL;
}


\"  {
  yy_push_state(TEXT_MODE);
} 

<TEXT_MODE>[[:alnum:]._/:\-]+|\\\" { 
  yylval.value = strdup(yytext);
  return TEXT;
}


<TEXT_MODE>\" {
  yy_pop_state();
 }


"["|"{"  {
  yy_push_state (TREE_MODE);
  return yytext[0];
 }

<TREE_MODE>"]"|"}" {
  yy_pop_state ();
  return yytext[0];
 }


<TREE_MODE,TEXT_MODE>[[:space:]\n] {}



.|\n { return yytext[0];}

%%
