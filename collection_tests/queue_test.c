#include "../include/collections/queue.h"
#include "../lib/obsidian.h"
#include "../include/llv.h"
#include "collection_test_helper.h"
#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"
#include <string.h>

int main(void) {
    OBS_SETUP("Queue")

    OBS_TEST_GROUP("Queue_new && queue_new_node", {
        OBS_TEST("Create queue and test properties", {
            Queue queue = queue_new("1");
            obs_test_strcmp(queue->parent.name, "1");
            test_empty_list(queue, queue);
            queue_free(queue);
        })

        OBS_TEST("Create node and test properties", {
            // NOTE: no point testing the other data members when selecting one
            // i.e. if we have set int_data = 4 we shouldn't test str_data
            // this is because it is classified as undefined behaviour and
            // we shouldn't expect undefined behaviour to behave in any way.

            // int node
            QueueNode node = queue_new_node((Data){.int_data = 4}, INTEGER);
            obs_test(node->data_tag, ==, INTEGER);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.int_data, ==, (long long)4);
            queue_free_node(node);

            // flt node
            node = queue_new_node((Data){.flt_data = 5.9}, FLOAT);
            obs_test(node->data_tag, ==, FLOAT);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.flt_data, ==, 5.9);
            queue_free_node(node);

            // str node
            node = queue_new_node((Data){.str_data = "Hello"}, STRING);
            obs_test(node->data_tag, ==, STRING);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            queue_free_node(node);

            // any node
            int x = 9;
            node = queue_new_node((Data){.any_data = &x}, ANY);
            obs_test(node->data_tag, ==, ANY);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test((int*)node->data.any_data, ==, &x);
            queue_free_node(node);
        })

        OBS_TEST("Create node using generic macros", {
            // int node
            QueueNode node = NEW_NODE(queue, 4);
            obs_test(node->data_tag, ==, INTEGER);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.int_data, ==, (long long)4);
            queue_free_node(node);

            // flt node
            node = NEW_NODE(queue, 5.9);
            obs_test(node->data_tag, ==, FLOAT);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.flt_data, ==, 5.9);
            queue_free_node(node);

            // str node
            node = NEW_NODE(queue, "Hello");
            obs_test(node->data_tag, ==, STRING);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            queue_free_node(node);

            // any node
            int x = 9;
            node = NEW_NODE(queue, &x);
            obs_test(node->data_tag, ==, ANY);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test((int*)node->data.any_data, ==, &x);
            queue_free_node(node);
        })
    });

    OBS_TEST_GROUP("Queue General", {
        OBS_TEST("Empty queue then adding elements", {
            Queue queue = queue_new("1");
            test_empty_list(queue, queue);
            queue_enqueue(queue, NEW_NODE(queue, 1));
            obs_test(queue_length(queue), ==, (size_t)1);
            obs_test_false(queue_is_empty(queue));

            queue_free_node(queue_dequeue(queue));
            obs_test(queue_length(queue), ==, (size_t)0);
            obs_test_true(queue_is_empty(queue));
            queue_free(queue);
        })

        OBS_TEST("Large queue", {
            Queue queue = queue_new("2");
            test_empty_list(queue, queue);
            for (int i = 0; i < 50; i++) {
                queue_enqueue(queue, NEW_NODE(queue, i));
                obs_test(queue_length(queue), ==, (size_t)(i + 1));
                obs_test_false(queue_is_empty(queue));
            }

            for (int i = 0; i < 50; i++) {
                QueueNode n = queue_dequeue(queue);
                obs_test(n->data.int_data, ==, (long long)i);
                obs_test(queue_length(queue), ==, (size_t)(49 - i));
            }
            obs_test_true(queue_is_empty(queue));
        })
    })

    OBS_TEST_GROUP("Queue clear", {
        OBS_TEST("Clear empty", {
            Queue queue = queue_new("1");
            test_empty_list(queue, queue);
            queue_clear(queue);
            test_empty_list(queue, queue);
        })

        OBS_TEST("Clear with elements", {
            Queue queue = queue_new("1");
            test_empty_list(queue, queue);
            for (int i = 0; i < 100; i++) {
                queue_enqueue(queue, NEW_NODE(queue, i));
            }
            obs_test(queue_length(queue), ==, (size_t)100);
            queue_clear(queue);
            test_empty_list(queue, queue);
        })
    })

    OBS_REPORT
}