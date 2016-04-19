%{
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "parser.tab.h"
#include <util_parser.h>
#include <ast.h>
#define MAX_SIZE 500
  
char* strdup(const char *);
int fileno(FILE *);

char* convert_to_html(char* string);

%}


label ((_[[:alpha:]]|[[:alpha:]])[[:alnum:]._\']*)
xml (?i:xml){label}*

mot (([^\"[:space:]]|\\\")+)

comparaison ("=="|"<"|">"|"<="|">=")

binary ("||"|"&&")

%x TEXT_MODE 

%option noinput nounput 

%%

"_"/"{" { yylval.str = strdup(yytext);return T_FILTER_ACC; }

"_" { yylval.str = strdup(yytext);return T_FILTER; }


"let"  { return T_LET; }

"emit" { yylval.str = strdup(yytext); return T_EMIT; }

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

{comparaison}  {
  yylval.str = strdup(yytext);
  return T_COMP;
}

{binary}      {
  yylval.str = strdup(yytext);
  return T_BINARY;
}


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
  struct ast* tree = mk_tree("", true, false,false, NULL,nb);
  yylval.node = tree;
  return T_NB;
}

\"  { BEGIN TEXT_MODE;}

<TEXT_MODE>{mot}/[[:space:]]+ {
  struct ast* word = mk_word(strdup(yytext));
  struct ast* tree = mk_tree("",true,false,true,NULL,word);
  yylval.node = tree;
  return T_TEXT;
}

<TEXT_MODE>{mot} {
  struct ast* word = mk_word(convert_to_html(yytext));
  struct ast* tree = mk_tree("", true,false,false,NULL,word);
  yylval.node = tree;
  return T_TEXT;
}

<TEXT_MODE>\" { BEGIN INITIAL;}

<INITIAL,TEXT_MODE>[[:space:]\n] {}

"]/"  {return T_END_ATTRIBUT;}

. { printf("%c",yytext[0]);return yytext[0]; }


%%


char* convert_to_html(char* string){
  char * base = "&#x";
  char final_string[MAX_SIZE];
  int j=0;
  for(unsigned long i = 0 ; i < strlen(string) ; i++){
    if((65 < string[i] && string[i] < 90) || (97 < string[i] && string[i] < 122)){ //Char entre a-z ou A-Z
      final_string[j] = string[i];
      j++;
    }
    else if(string[i] != 32){
      sprintf(final_string+j, "%s%d;", base, string[i]);
      while(final_string[j]!='\0')
        j++;
    }
  }
  final_string[j]='\0';
  return strdup(final_string);
}
