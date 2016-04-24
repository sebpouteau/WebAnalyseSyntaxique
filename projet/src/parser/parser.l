%{
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "parser.tab.h"
#include <ast.h>
#define MAX_SIZE 500
  
char* strdup(const char *);
int fileno(FILE *);

char* convert_to_html(char* string);
struct dir* mk_dir(char* name, enum descr type, struct dir* next);
struct path* split_path(char * str);
struct dir* add_dir_right(struct dir* src, struct dir* next);
struct path* mk_path(int srep, struct dir* d);

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

\$\.*.+  { struct path* p = split_path(yytext); yylval.node = mk_import(p); return PATH; }

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
  struct ast* tree = mk_tree("digit", true, false,false, NULL,nb);
  yylval.node = tree;
  return T_NB;
}

\"  { BEGIN TEXT_MODE;}

<TEXT_MODE>{mot}/[[:space:]]+ {
  struct ast* word = mk_word(strdup(yytext));
  struct ast* tree = mk_tree("word",true,false,true,NULL,word);
  yylval.node = tree;
  return T_TEXT;
}

<TEXT_MODE>{mot} {
  struct ast* word = mk_word(convert_to_html(yytext));
  struct ast* tree = mk_tree("word", true,false,false,NULL,word);
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



struct dir* mk_dir(char* name, enum descr type, struct dir* next){
  struct dir* d = malloc(sizeof(*d));
  d->str = name;
  d->descr = type;
  d->dir = next;
  return d;
}

struct path* mk_path(int srep, struct dir * d){
  struct path* p = malloc(sizeof(*p));
  p->n = srep;
  p->dir = d;
  return p;
}

struct dir* add_dir_right(struct dir* src, struct dir* next){
  if(src == NULL){
    return next;
  }
  
  struct dir* tmp = src;
  while(tmp->dir != NULL){
    tmp = tmp->dir;
  }
  tmp->dir = next;
  return src;
}


struct path* split_path(char * str){
  struct dir* src = NULL;
  int srep = 0;
  char* dir_name = malloc(200*sizeof(char));
  int count = 0;
  unsigned long i = 0;
  
  //on compte le nombre de retour
  while(str[i] == '.'){
    srep++;
    i++;
  }
  
  for(  ; i < strlen(str) ; i++){
    if (str[i] == '/'){
      src = add_dir_right(src, mk_dir(strdup(dir_name), DIR, NULL));    
      memset(dir_name, 0, sizeof(*dir_name));
      count = 0;
    }
    else {
      dir_name[count] = str[i];
      count++;
    }
  }
  
  char* file_name = malloc(200 * sizeof(char));
  count = 0;
  unsigned long j = 0;
  for( ; j < strlen(dir_name); j++){
    if(j+1 < strlen(dir_name) && dir_name[j] == '-' && dir_name[j+1] == '>'){
      j+=2;
      break;
    } else {
      file_name[count] = dir_name[j];
      count++;
    }
  }
  
  src = add_dir_right(src, mk_dir(strdup(file_name), FILENAME, NULL));
  memset(file_name, 0, sizeof(*file_name));
  count = 0;
  
  for( ; j < strlen(dir_name); j++){
    file_name[count] = dir_name[j];
    count++;
  }
  
  if(count != 0){
    src = add_dir_right(src, mk_dir(strdup(file_name), DECLNAME, NULL));
  }

  struct path* p = mk_path(srep, src);
  free(file_name);
  free(dir_name);
  return p;
}
