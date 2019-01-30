#ifndef LLV_QUEUE_H
#define LLV_QUEUE_H

#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"
#include "ll_structs.h"

typedef struct _singly_linked_list_t *Queue;
typedef struct _LL_node_t *QueueNode;

/* Create a new queue with a given name */
Queue queue_new(char *name);

/* Frees the queue and all the memory its allocated */
void queue_free(Queue queue);

/* Frees the given node */
void queue_free_node(QueueNode n);

/* Returns how many items are currently in queue */
int queue_length(Queue queue);

/* Returns true if there are no items in queue */
int queue_is_empty(Queue queue);

/* Free all nodes in queue. */
void queue_clear(Queue queue);

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

#endif /* LLV_QUEUE_H */
