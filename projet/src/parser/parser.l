%{
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "parser.tab.h"
#include <util_parser.h>

char* strdup(const char *);
int fileno(FILE *);


%}


label ((_[[:alpha:]]|[[:alpha:]])[[:alnum:]._]*)

xml (i:xml)?({label})*
mot ([[:alnum:]._/:\-\'!?;]+|\\\")

cond ("=="|"<"|">"|"<="|">="|"||"|"&&"|"!")

%x TEXT_MODE

%option noinput nounput 

%%

"_" {return FILTER;}

"_"/[[:blank:]] {return FILTER_SPACE;}

"let"  {  printf("je passe dans let\n");return LET; }

"emit" {  return EMIT; }

"in" {  return IN; }

"where" {  return WHERE; }

"fun" {  return FUN; }

"->" {  return ARROW; }

"if" {  return IF; }

"then" {  return THEN; }

"else" {  return ELSE; }

"match" {  return MATCH; }

"with" {  return WITH; }

"end"  {return END_MATCH;}


{cond}  {/*yylval.value = strdup(yytext);*/ return COND;}

{label}/[/\[\{]  {
  //printf("je passe dans LABEL\n");
  // yylval.value = strdup(yytext);
  return LABEL;
}

{label} {
  //printf("je passe dans VAR\n");
      
  // yylval.value = strdup(yytext);
  return VAR;
}




{xml} {
  //yylval.value = strdup(yytext);
  return XML;
}

[[:digit:]]+ {/*yylval.number = atoi(yytext);*/ return NB;}

\"  { BEGIN TEXT_MODE;}

<TEXT_MODE>{mot}/[[:space:]]+ {
  // yylval.n_text = text_create(yytext,true);
  return TEXT;
}

<TEXT_MODE>{mot} {
  // printf("mot\n");
  // yylval.n_text = text_create(yytext,false);;
  return TEXT;
}

<TEXT_MODE>\" { BEGIN INITIAL;}


<INITIAL,TEXT_MODE>[[:space:]\n] {}

\n { return EOL; }

. { return yytext[0]; }
