#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/ll.h"
#include "../../include/collections/queue.h"
#include "../../include/helper.h"
#include "../list_helper.h"

Queue queue_new(char *name) {
    return ll_new(name);
}

void queue_free(Queue queue) {
    ll_free(queue);
}

void queue_free_node(QueueNode n) {
    ll_free_node(n);
}

void queue_clear(Queue queue) {
    ll_clear(queue);
}

size_t queue_length(Queue queue) {
    return ll_length(queue);
}

bool queue_is_empty(Queue queue) {
    return ll_is_empty(queue);
}

QueueNode queue_new_node(Data data, TypeTag type) {
    return ll_new_node(data, type);
}

void queue_enqueue(Queue queue, QueueNode node) {
    ll_append(queue, node);
}

QueueNode queue_dequeue(Queue queue) {
    return ll_pop(queue);
}
