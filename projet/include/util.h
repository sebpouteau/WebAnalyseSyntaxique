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
 *  \param struct queue_t*
 *  \param struct ast*, 
 *  \return return struct declare*
 */
struct declare*  mk_declare(struct queue_t* q, struct ast* a);

/**
 *  \brief Renvoie le type binop d'un string
 *
 * Par exemple si on passe "==" il nous retourne EQ
 *
 *  \param char*  chaine à analyser 
 *  \return return struct ast*
 */
struct ast* mk_comp_ast(char * str);

/**
 *  \brief Constructeur de la structure patterns
 *  \param struct pattern* 
 *  \param struct ast* 
 *  \param struct pattern*  structure suivant
 *  \return return struct patterns*
 */
struct patterns* mk_patterns(struct  pattern * pattern, struct ast * res, struct patterns * next);

/**
 *  \brief Constructeur de la structure path
 *  \param int  le nombre de retour en arrière
 *  \param struct dir*
 *  \return return struct path*
 */
struct path* mk_path(int srep, struct dir* d);

/**
 *  \brief Constructeur de la structure dir
 *  \param char nom
 *  \param enum descr  type
 *  \param struct dir*  structure suivante
 *  \return return struct dir*
 */
struct dir* mk_dir(char* name, enum descr type, struct dir* next);

/**
 *  \brief Constructeur de la structure attributes
 *  \param struct ast* key 
 *  \param struct ast*  value
 *  \param struct attributes*  structure suivante
 *  \return return struct attributes*
 */
struct attributes* mk_attributes(struct ast* key, struct ast* value, struct attributes* next);

/**
 *  \brief permet d'ajouter un frere à une structure dir
 *  \param struct dir* noeud initial
 *  \param struct dir* frere a ajouter
 *  \return return struct dir*
 */
struct dir* add_dir_right(struct dir* src, struct dir* next);

/**
 *  \brief permet d'ajouter un frere à une structure ast
 *  \param struct ast* noeud initial
 *  \param struct ast* frere a ajouter
 *  \return return struct ast*
 */
struct ast* add_right(struct ast* frst, struct ast* element);

/**
 *  \brief permet d'ajouter un frere à une structure pattern
 *  \param struct patterns* noeud initial
 *  \param struct patterns* frere a ajouter
 */
void add_patterns_right(struct patterns* filter, struct patterns* element);

/**
 *  \brief permet d'ajouter un frere à une structure pattern
 *  \param struct pattern* noeud initial
 *  \param struct pattern* frere a ajouter
 */
void add_pforest_right(struct pattern* filter, struct pattern* element);

/**
 *  \brief permet d'ajouter un frere à une structure ast
 *  Cette fonction nous sert à ajouter les arbres à notre variable globale
 *  \param struct ast* noeud initial
 *  \param struct ast* frere a ajouter
 *  \param struct env* environnemet du fichier
 */
void add_head(struct ast* head, struct ast* head_tree, struct env* e);

/**
 *  \brief permet de convertir un char* en char* valide pour du html
 *  \param bool sert à savoir si oui ou non on est dans le cas d'un text de EMIT
 *  \param char* chaine à traduire
 *  \return char* traduction de l'ancienne chaine (renvoie une copie)
 */
char* convert_to_html(bool* is_emit, char* string);

/**
 *  \brief permet de remplir une struct path* grace à une chaine de caractère
 *  \param char* chaine à transformer
 *  \return struct path* 
 */
struct path* split_path(char * str);

/**
 *  \brief permet créer une struct ast* à partir d'une struct declaration
 *  Utiliser pour crèer l'arbre des fonctions
 *  \param struct declare* il contient la queue_t d'argument et le body de la fonction
 *  \param char* nom de la fonction
 *  \param bool si oui ou non la fonction est recursive
 *  \return return struct ast*
 */
struct ast* create_args(struct declare* decl, char*name, bool is_rec);

/**
 *  \brief permet créer une struct ast* à partir d'une struct declaration dans le cas d'un IN
 *  Utiliser pour crèer l'arbre des fonctions present dans un IN, elle fait appel a la fonction précédente
 *  \param struct ast*  corps apres le IN
 *  \param struct declare* il contient la queue_t d'argument et le body de la fonction
 *  \param bool si oui ou non la fonction est recursive
 *  \return return struct ast*
 */
struct ast* create_ast_in(struct ast* body, struct declare* decl, bool is_rec);



#endif
