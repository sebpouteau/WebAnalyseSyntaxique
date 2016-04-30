%{
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "parser.tab.h"
#include <ast.h>
#include <util.h>

  
char* strdup(const char *);
int fileno(FILE *);

bool is_emit = false;
%}


label ((_[[:alpha:]]|[[:alpha:]])[[:alnum:]._\']*)
xml (?i:xml){label}*

mot (([^\"[:space:]]|\\\")+)

comparaison ("=="|"<"|">"|"<="|">="|"!=")

binary ("||"|"&&")

%x TEXT_MODE 

%option noinput nounput 

%%

"_"/"{" { yylval.str = strdup(yytext);return T_FILTER_ACC; }

"_" { yylval.str = strdup(yytext);return T_FILTER; }

"let"  { return T_LET; }

"emit" { is_emit = true; yylval.str = strdup(yytext); return T_EMIT; }

"in" {  return T_IN; }

"where" {  return T_WHERE; }

"fun" {  return T_FUN; }

"->" { return T_ARROW; }

"if" {  return T_IF; }

"then" {  return T_THEN; }

"else" {  return T_ELSE; }

"match" { return T_MATCH; }

"with" {  return T_WITH; }

"end"  { return T_END_MATCH; }

"rec"  { return T_REC; }

{comparaison}  {
  yylval.str = strdup(yytext);
  return T_COMP;
}

{binary}      {
  yylval.str = strdup(yytext);
  return T_BINARY;
}

\$\.*.+/";"  { struct path* p = split_path(yytext); yylval.node = mk_import(p); return PATH; }

{xml} {
  yylval.str = strdup(yytext);
  return T_VAR;
}

{xml}/[/\[\{] {
  yylval.str = strdup(yytext);
  return T_ERROR;
}

{label}/[/\[\{]  {
  yylval.str = strdup(yytext);
  return T_LABEL;
}

{label} {
  yylval.str = strdup(yytext);
  return T_ATTRIBUT;
}

[[:digit:]]+ {
  struct ast* nb = mk_integer(atof(yytext));
  yylval.node = nb;
  return T_NB;
}

\"  { BEGIN TEXT_MODE;}

<TEXT_MODE>{mot}/[[:space:]]+ {
  struct ast* word = mk_word(convert_to_html(&is_emit, yytext));
  struct ast* tree = mk_tree("word",true,false,true,NULL,word);
  yylval.node = tree;
  return T_TEXT;
}

<TEXT_MODE>{mot} {
  struct ast* word = mk_word(convert_to_html(&is_emit, yytext));
  struct ast* tree = mk_tree("word", true,false,false,NULL,word);
  yylval.node = tree;
  return T_TEXT;
}

<TEXT_MODE>\" { BEGIN INITIAL;}

<INITIAL,TEXT_MODE>[[:space:]\n] {}

"]/"  {return T_END_ATTRIBUT;}

. { return yytext[0]; }


%%


