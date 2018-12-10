#ifndef LLV_QUEUE_H
#define LLV_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "../types/shared_types.h"
#include "../types/collection_skeleton.h"

typedef struct _singly_linked_list_t *Queue;
typedef struct _LL_node_t *QueueNode;

Queue queue_new(char *name);

void queue_free(Queue queue);

QueueNode queue_new_node(Data data, TypeTag type);

void queue_enqueue(Queue queue, QueueNode node);

QueueNode queue_dequeue(Queue queue);

#endif