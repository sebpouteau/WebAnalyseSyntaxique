%{
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "parser.tab.h"
#include <util_parser.h>
  
char* strdup(const char *);
int fileno(FILE *);


%}


label (_[[:alpha:]]|[[:alpha:]])[[:alnum:]._]*
xml (i:xml)?({label})*

%x TEXT_MODE
%s TREE_MODE LET_MODE EMIT_MODE

%option noyy_top_state
%option noinput nounput stack

%%

"let"  {
  yy_push_state(LET_MODE);
  return LET;
}

"emit" {
  yy_push_state(EMIT_MODE);
  return EMIT;
}

"in"[[:space:]]+ {
yy_pop_state();
return IN;
}

[[:blank:]]*"where"[[:space:]\n]* {
  yy_push_state(LET_MODE);
  return WHERE;
}

<LET_MODE>"fun" {
  return FUN;
 }

<LET_MODE>{xml} {
  yylval.value = strdup(yytext);
  return LABEL;
}

<TREE_MODE>{xml} {
  yylval.value = strdup(yytext);
  return LABEL;
}

<TREE_MODE>"," {
  return VIRGULE;
}

<INITIAL,LET_MODE>"=" {
  return AFFECT;
}

<LET_MODE>";"  {
  yy_pop_state();
  return END_LET;
 }

{label}   {
  yylval.value = strdup(yytext);
  return LABEL;
}

{xml} {}


\"  {
  yy_push_state(TEXT_MODE);
} 

<TEXT_MODE>([[:alnum:]._/:\-]+|\\\")/[[:space:]]+ {
  yylval.n_text = text_create(yytext,true);
  return TEXT;
}

<TEXT_MODE>[[:alnum:]._/:\-]+|\\\" { 
  yylval.n_text = text_create(yytext,false);;
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


<TREE_MODE,TEXT_MODE,LET_MODE>[[:space:]\n] {}

<EMIT_MODE>\n {yy_pop_state(); return yytext[0];}
<EMIT_MODE>[[:space:]] {}

\n {return EOL;}
. { return yytext[0];}

%%


