%{
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "parserGrammaireNew.tab.h"
#include <util_parser.h>
  
char* strdup(const char *);
int fileno(FILE *);


%}


label (_[[:alpha:]]|[[:alpha:]])[[:alnum:]._]*
xml (i:xml)?({label})*
mot ([[:alnum:]._/:\-\'!?]+|\\\")

cond ("=="|"<"|">"|"<="|">="|"||"|"&&"|"!")

%x TEXT_MODE
%s SPACE_MODE

%option noyy_top_state
%option noinput nounput stack

%%

"_" {return FILTER;}

"_"/[[:blank:]] {return FILTER_SPACE;}

"let"  {  return LET; }

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

{label}  {
  // yylval.value = strdup(yytext);
  return LABEL;
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
  printf("mot\n");
  // yylval.n_text = text_create(yytext,false);;
  return TEXT;
}

<TEXT_MODE>\" { BEGIN INITIAL;}


"["|"{"  {
  BEGIN SPACE_MODE;
  return yytext[0];
 }

<SPACE_MODE>"]"|"}" {
  BEGIN INITIAL;
  return yytext[0];
 }

<TEXT_MODE,SPACE_MODE>[[:space:]\n] {}

\n { return EOL; }

. { return yytext[0]; }
