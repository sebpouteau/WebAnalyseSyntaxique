%{
#include <string.h>
#include "web.tab.h"

%}


label (_[[:alpha:]]|[[:alpha:]])[[:alpha:]\'._]*
xml (?i:xml)({label})*

%x TEXT_MODE

%%
  
{label}   {
  yylval = strdup(yytext);
  return LABEL;
}

\"  {BEGIN TEXT_MODE;}


<TEXT_MODE>\" {BEGIN INITIAL;}

<TEXT_MODE>[[:alnum:]]+  {
  yylval = strdup(yytext);
  return TEXT;
}

. { return yytext[0];}

%%
