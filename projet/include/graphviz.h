/**
 *  \file graphviz.h
 *  \brief Fichier regrouppant l'ensemble des fonctions utilisées pour générer les graphes.
 *  \author POUTEAU Sébastien <br>
 *  PALARD Nicolas <br>
 *  GOURAUD Jimmy <br>
 *  KIROV Yordan <br>
 *  ROBERT Etienne
 */

#ifndef GRAPHVIZ_H
#define GRAPHVIZ_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ast.h>

/**
 *  \brief Simple compteur <br>
 *  La fonction utilise un static int pour conserver sa valeur entre deux appels
 *  Cela évite d'utiliser une variable global pour compter
 *  \return cpt La valeur du compteur incrémentée
 */
int cpt(void);

/**
 *  \brief Cherche et renvoi l'id dans l'énumération du type passé en paramètre
 *  \param char* name Le nom du type (INTEGER, ...)
 *  \return enum ast_type La position du type cherché dans l'enumération
 */
enum ast_type find_ast_type(char *name);

/**
 *  \brief Cherche et renvoi le nom de l'id du type passé en paramètre
 *  \param enum ast_type id L'id du type dans l'énumération 
 *  \return char* Le nom sous forme d'une chaîne de caractères du type
 */
char* name_ast_type(enum ast_type id);

/**
 *  \brief Cherche et renvoi l'id de l'opérateur binaire passé en paramètre
 *  \param char*  Le nom de l'opérateur binaire
 *  \return enum binop La position de l'opérateur cherché dans l'enumération
 */
enum binop find_binop(char *name);

/**
 *  \brief Cherche et renvoi le nom de l'id d'opérateur binaire passé en paramètre
 *  \param enum binop id L'id de l'opérateur dans l'énumération
 *  \return char* Le nom sous forme d'une chaîne de caractères de l'opérateur
 */
char* name_binop(enum binop id);

/**
 *  \brief La fonction draw est la première à être appelée pour dessiner les graphes des arbres.
 *   Elle ouvre le fichier ou va être écris la syntaxe graphviz puis elle appelle draw_ast
 *   qui va rediriger le travail entre les différentes fonctions.
 *  \param char* name Le nom du fichier dans lequel la syntaxe sera écrite.
 *  \param struct ast* a Le premier noeud a être évalué
 *  \return void
 */
void draw(char* name, struct ast* a);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type IMPORT
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct ast* a La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_import(FILE * fd, struct ast * a, char * name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type DIR
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct dir* d La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_dir(FILE * fd, struct dir * d, char * name_parent);

/**
 *  \brief Génere la syntaxe graphviz en fonction du type de n'importe quelle structure ast. 
 *  Cette fonction génère les cas simples et réfere vers d'autres fonctions
 *  pour des cas plus compliqués (TREE, MATCH, FUN, ...)
 *  \param FILE* fd Le descripteur du fichier où écrire
 *  \param struct ast* a La structure contenant les données
 *  \param char*  name_parent Le nom du noeud parent
 *  \return void
 */
void draw_ast(FILE * fd, struct ast* a, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type TREE
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct tree* t La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_tree(FILE * fd, struct tree* t, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type MATCH
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct match* m La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_match(FILE * fd, struct match* m, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type COND
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct cond* c La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_cond(FILE * fd, struct cond *c, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type FUN
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct fun* f La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_fun(FILE * fd, struct fun* f, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type PATTERNS
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct patterns* p La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_patterns(FILE * fd, struct patterns* p, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type PATTERN
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct pattern* p La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_pattern(FILE * fd, struct pattern* p, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type APP
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct app* a La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_app(FILE * fd, struct app* a, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type FOREST
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct forest* f La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_forest(FILE * fd, struct forest* f, char* name_parent);

/**
 *  \brief Génere la syntaxe graphviz pour un noeud de type ATTRIBUTES
 *  \param FILE* fd Le descripteur de fichier où écrire
 *  \param struct attributes* a La structure contenant les données
 *  \param char* name_parent Le nom du noeud parent
 *  \return void
 */
void draw_attributes(FILE * fd, struct attributes * a, char* name_parent);

#endif

