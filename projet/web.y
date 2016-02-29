%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
int yylex(void);
void yyerror(char *);
int yyparse(void);

struct tree;
struct attributes;
enum type {tree, word};        //typage des nœuds: permet de savoir si un nœud construit
                               //un arbre ou s'il s'agit simplement de texte
struct attributes{
    char * key;               //nom de l'attribut
    char * value;             //valeur de l'attribut
    struct attributes * next; //attribut suivant
};

struct tree {
    char * label;              //étiquette du nœud
    bool nullary;              //nœud vide, par exemple <br/>
    bool space;                //nœud suivi d'un espace
    enum type tp;              //type du nœud. nullary doit être true si tp vaut word
    struct attributes * attr;  //attributs du nœud
    struct tree * daughters;   //fils gauche, qui doit être NULL si nullary est true
    struct tree * right;       //frère droit
};

%}


%token LABEL
%token TEXT
                        
%%


content:        LABEL '{' content '}' {
                  printf("label {content}\n");
                }

        |       LABEL '[' attributes "]/"  {
                  printf("label [attribut]\n");
                }
        |       LABEL '[' attributes ']' '{' text '}'
                {
                  printf("label [attribut]{text}\n");
                }
        |       '{' content '}'
                {
                  printf("{content}\n");
                }
        |       text
                {
                  printf("test\n");
                }
        |       /* empty */
        ;

attributes:     LABEL '=' text attributes
        |       /* empty */
        ;


text:           TEXT
        |       /* empty */
        ;
%%



int main(void)
{
  yyparse();
  return 0;
}

