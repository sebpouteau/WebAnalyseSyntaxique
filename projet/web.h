#ifndef WEB_H
#define WEB_H
#include <stdbool.h>

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

#endif
