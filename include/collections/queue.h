#ifndef LLV_QUEUE_H
#define LLV_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _singly_linked_list_t *Queue;
typedef struct _LL_node_t *QueueNode;

/* Create a new queue with a given name */
Queue queue_new(char *name);

/* Frees the queue and all the memory its allocated */
void queue_free(Queue queue);

/*
   Create a new node
   Could use NEW_NODE(queue, data)
*/
QueueNode queue_new_node(Data data, TypeTag type);

/*
    Adds a object to the end of the queue
*/
void queue_enqueue(Queue queue, QueueNode node);

/*
    Takes an object from the front of the queue.
*/
QueueNode queue_dequeue(Queue queue);

#endif
