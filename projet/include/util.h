/**
 *  \file util.h
 *  \brief Enssemble de fonctions intermédiaires. 
 *  \author POUTEAU Sébastien <br>
 *  PALARD Nicolas <br>
 *  GOURAUD Jimmy <br>
 *  KIROV Yordan <br>
 *  ROBERT Etienne
 */

#ifndef UTIL_H
#define UTIL_H
#include <queue.h>
#include <ast.h>
#include <machine.h>


struct declare{
  struct queue_t* args;
  struct ast* body;
};


/**
 *  \brief Constructeur de la structure declare
 *  \param struct queue_t* q
 *  \param struct ast* a 
 *  \return struct declare*
 */
struct declare* mk_declare(struct queue_t* q, struct ast* a);

/**
 *  \brief Renvoie le type binop d'un string
 *
 * Par exemple si on passe "==" il nous retourne EQ
 *
 *  \param char*  chaine à analyser 
 *  \return struct ast*
 */
struct ast* mk_comp_ast(char * str);

/**
 *  \brief Constructeur de la structure patterns
 *  \param struct pattern* 
 *  \param struct ast* 
 *  \param struct pattern*  structure suivant
 *  \return struct patterns*
 */
struct patterns* mk_patterns(struct  pattern * pattern, struct ast * res, struct patterns * next);

/**
 *  \brief Constructeur de la structure path
 *  \param int srep le nombre de sous répertoires à visiter
 *  \param struct dir* d
 *  \return struct path*
 */
struct path* mk_path(int srep, struct dir* d);

/**
 *  \brief Constructeur de la structure dir
 *  \param char* name
 *  \param enum descr type
 *  \param struct dir* next  le repertoir suivant
 *  \return struct dir*
 */
struct dir* mk_dir(char* name, enum descr type, struct dir* next);

/**
 *  \brief Constructeur de la structure attributes
 *  \param struct ast* key 
 *  \param struct ast*  value
 *  \param struct attributes* next l'attribut suivant dans la liste chaînée
 *  \return struct attributes*
 */
struct attributes* mk_attributes(struct ast* key, struct ast* value, struct attributes* next);

/**
 *  \brief Permet d'ajouter un frere à une structure dir
 *  \param struct dir* src le noeud initial
 *  \param struct dir* next le frere à ajouter
 *  \return struct dir*
 */
struct dir* add_dir_right(struct dir* src, struct dir* next);

/**
 *  \brief Permet d'ajouter un frere à une structure ast
 *  \param struct ast* first le noeud initial
 *  \param struct ast* element le frere à ajouter
 *  \return struct ast*
 */
struct ast* add_right(struct ast* frst, struct ast* element);

/**
 *  \brief Permet d'ajouter un frere à une structure patterns
 *  \param struct patterns* filter le noeud initial
 *  \param struct patterns* element le frere à ajouter
 */
void add_patterns_right(struct patterns* filter, struct patterns* element);

/**
 *  \brief Permet d'ajouter un frere à une structure pattern
 *  \param struct pattern* filter le noeud initial
 *  \param struct pattern* element le frere à ajouter
 */
void add_pforest_right(struct pattern* filter, struct pattern* element);

/**
 *  \brief Permet d'ajouter un frere à une structure ast <br>
 *  Cette fonction nous sert à ajouter les arbres à notre variable globale
 *  \param struct ast* head le noeud initial
 *  \param struct ast* head_tree le frere à ajouter
 *  \param struct env* environnemet du fichier
 */
void add_head(struct ast* head, struct ast* head_tree, struct env* e);

/**
 *  \brief Permet de convertir un char* en char* valide pour du html
 *  \param bool is_emi Permet de  savoir si oui ou non on est dans le cas d'un text de EMIT (Si on est dans un emit, il ne faut pas convertir la chaine => nom du fichier)
 *  \param char* string la chaine à convertir
 *  \return char* Traduction de l'ancienne chaine (renvoie une copie)
 */
char* convert_to_html(bool* is_emit, char* string);

/**
 *  \brief Permet de remplir une struct path* grace à une chaine de caractère
 *  \param char* str la chaine à partir de laquelle on peut génrer la structure de données
 *  \return struct path* la nouvelle structure générée
 */
struct path* split_path(char * str);

/**
 *  \brief Permet créer une struct ast* à partir d'une struct declaration <br>
 *  Utilisé pour crèer l'arbre des fonctions
 *  \param struct declare* decl Cette structure contient la queue_t d'argument et le body de la fonction
 *  \param char* name le nom de la fonction
 *  \param bool is_rec indique si la fonction est récursive
 *  \return struct ast*
 */
struct ast* create_args(struct declare* decl, char* name, bool is_rec);

/**
 *  \brief Permet créer une struct ast* à partir d'une struct declaration dans le cas d'un IN <br>
 *  Utilisé pour crèer l'arbre des fonctions présent dans un IN, elle fait appelle à la fonction précédente
 *  \param struct ast* body Partie qui se trouve apres le "IN"
 *  \param struct declare* decl Cette structure contient la queue_t d'argument et le body de la fonction
 *  \param bool is_rec indique si la fonction est récursive
 *  \return struct ast*
 */
struct ast* create_ast_in(struct ast* body, struct declare* decl, bool is_rec);



#endif
