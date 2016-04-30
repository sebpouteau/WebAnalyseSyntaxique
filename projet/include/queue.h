/**
 *  \file queue.h
 *  \brief Fichier regrouppant l'ensemble des fonctions utilisées pour gérer une file.
 *  \author POUTEAU Sébastien <br>
 *  PALARD Nicolas <br>
 *  GOURAUD Jimmy <br>
 *  KIROV Yordan <br>
 *  ROBERT Etienne
 */

#ifndef QUEUE_H
#define QUEUE_H

struct queue_t;

typedef struct cell_t* cell;

struct cell_t{
  cell pred;
  void* value;
};

struct queue_t{
  cell head;
  cell tail;
};

typedef struct queue_t *queue;

/**
 *  \brief Constructeur de file vide
 *  \return queue la queue créée
 */
queue mk_queue(void);

/**
 *  \brief Vide et détruit la file passée en parmètre.
 *  \param queue q la file à vider
 *  \return void
 */
void queue_destroy(queue q);

/**
 *  \brief Retourne vrai si la file est vide
 *  \param queue q
 *  \return int un booléen indiquant si la file est vide ou non
 */
int queue_empty(queue q);

/**
 *  \brief Ajoute un object dans la file
 *  \param queue q
 *  \param void* object l'objet a ajouté => File générique
 *  \return void
 */
void queue_push(queue q, void *object);

/**
 *  \brief Retourne l'objet en tête de file
 *  \param queue q
 *  \return void* l'objet en tête de file
 */
void* queue_front(queue q);

/**
 *  \brief Supprime l'élement en tête de file (FIRST IN FIRST OUT)
 *  \param queue q
 *  \return void
 */
void queue_pop(queue q);

#endif
