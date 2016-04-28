#include<stdio.h>
#include<stdlib.h>
#include <queue.h>




cell mk_cell(){
  cell c = malloc(sizeof(*c));
  c->pred = NULL;
  c->value = NULL;
  return c;
}

/* create an empty queue */
queue mk_queue(void){
  queue q = malloc(sizeof(*q));
  cell c = mk_cell();
  q->head = c;
  q->tail = c;
  return q;
}

/* create an empty queue */
void queue_destroy(queue q){
  while (!queue_empty(q)){
    queue_pop(q);
  }
  free(q->head);
  free(q);
}

/* return true if and only if the queue is empty */
int queue_empty(queue q){
  return q->head->value == NULL;
}

/* push an object on the back of the queue */
void queue_push(queue q, void *object){
  q->tail->value = object;
  cell c = mk_cell();
  q->tail->pred = c;
  c->pred = NULL;
  q->tail = c;
}

/* return the front element of the queue.
   The queue must not be empty (as reported by queue_empty()) */
void * queue_front(queue q){
  return q->head->value;
}

/* pop the front element off of the queue.
   The queue must not be empty (as reported by queue_empty()) */
void queue_pop(queue q){
  cell c = q->head;
  q->head = q->head->pred;
  free(c);
}


