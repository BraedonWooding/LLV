#include "../include/collections/queue.h"
#include "../include/llv.h"

#define BIG_STRING "this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size.this is a very big string that is too big to handle on any size."

int main(void) {
    Queue queue = queue_new("Too big");
    queue_enqueue(queue, NEW_NODE(queue, 3));
    queue_enqueue(queue, NEW_NODE(queue, 1));
    queue_enqueue(queue, NEW_NODE(queue, BIG_STRING));
    update(1, queue);
    queue_free(queue);
}
