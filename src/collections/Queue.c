#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/collections/LL.h"
#include "../../include/collections/Queue.h"
#include "../../include/helper.h"
#include "../list_helper.h"

Queue queue_new(char *name) {
    return LL_new(name);
}

void queue_free(Queue queue) {
    LL_free(queue);
}

QueueNode queue_new_node(Data data, TypeTag type) {
    return LL_new_node(data, type);
}

void queue_enqueue(Queue queue, QueueNode node) {
    LL_append(queue, node);
}

QueueNode queue_dequeue(Queue queue) {
    return LL_pop(queue);
}
