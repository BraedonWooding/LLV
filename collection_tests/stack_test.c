#include "../include/collections/stack.h"
#include "../lib/obsidian.h"
#include "../include/llv.h"
#include "collection_test_helper.h"
#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"
#include <string.h>

int main(int argc, char *argv[]) {
    OBS_SETUP("Stack")

    OBS_TEST_GROUP("Stack_new && stack_new_node", {
        OBS_TEST("Create stack and test properties", {
            Stack stack = stack_new("1");
            obs_test_strcmp(stack->parent.name, "1");
            test_empty_list(stack, stack);
            stack_free(stack);
        })

        OBS_TEST("Create node and test properties", {
            // NOTE: no point testing the other data members when selecting one
            // i.e. if we have set int_data = 4 we shouldn't test str_data
            // this is because it is classified as undefined behaviour and
            // we shouldn't expect undefined behaviour to behave in any way.

            // int node
            StackNode node = stack_new_node((Data){.int_data = 4}, INTEGER);
            obs_test_eq(node->data_tag, INTEGER);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq(node->data.int_data, (long long)4);
            stack_free_node(node);

            // flt node
            node = stack_new_node((Data){.flt_data = 5.9}, FLOAT);
            obs_test_eq(node->data_tag, FLOAT);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq(node->data.flt_data, 5.9);
            stack_free_node(node);

            // str node
            node = stack_new_node((Data){.str_data = "Hello"}, STRING);
            obs_test_eq(node->data_tag, STRING);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            stack_free_node(node);

            // any node
            int x = 9;
            node = stack_new_node((Data){.any_data = &x}, ANY);
            obs_test_eq(node->data_tag, ANY);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq((int*)node->data.any_data, &x);
            stack_free_node(node);
        })

        OBS_TEST("Create node using generic macros", {
            // int node
            StackNode node = NEW_NODE(stack, 4);
            obs_test_eq(node->data_tag, INTEGER);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq(node->data.int_data, (long long)4);
            stack_free_node(node);

            // flt node
            node = NEW_NODE(stack, 5.9);
            obs_test_eq(node->data_tag, FLOAT);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq(node->data.flt_data, 5.9);
            stack_free_node(node);

            // str node
            node = NEW_NODE(stack, "Hello");
            obs_test_eq(node->data_tag, STRING);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            stack_free_node(node);

            // any node
            int x = 9;
            node = NEW_NODE(stack, &x);
            obs_test_eq(node->data_tag, ANY);
            obs_test_eq((void*)node->next, NULL);
            obs_test_eq((void*)node->ptr, NULL);
            obs_test_eq((int*)node->data.any_data, &x);
            stack_free_node(node);
        })
    });

    OBS_TEST_GROUP("Stack General", {
        OBS_TEST("Empty stack then adding elements", {
            Stack stack = stack_new("1");
            test_empty_list(stack, stack);
            stack_push(stack, NEW_NODE(stack, 1));
            obs_test_eq(stack_length(stack), (int)1);
            obs_test_false(stack_is_empty(stack));

            stack_free_node(stack_pop(stack));
            obs_test_eq(stack_length(stack), (int)0);
            obs_test_true(stack_is_empty(stack));
            stack_free(stack);
        })

        OBS_TEST("Large stack", {
            Stack stack = stack_new("2");
            test_empty_list(stack, stack);
            for (int i = 0; i < 50; i++) {
                stack_push(stack, NEW_NODE(stack, i));
                obs_test_eq(stack_length(stack), (int)(i + 1));
                obs_test_false(stack_is_empty(stack));
            }

            for (int i = 49; i >= 0; i--) {
                StackNode n = stack_pop(stack);
                obs_test_eq(n->data.int_data, (long long)i);
                obs_test_eq(stack_length(stack), (int)i);
            }
            obs_test_true(stack_is_empty(stack));
        })
    })

    OBS_TEST_GROUP("Stack clear", {
        OBS_TEST("Clear empty", {
            Stack stack = stack_new("1");
            test_empty_list(stack, stack);
            stack_clear(stack);
            test_empty_list(stack, stack);
        })

        OBS_TEST("Clear with elements", {
            Stack stack = stack_new("1");
            test_empty_list(stack, stack);
            for (int i = 0; i < 100; i++) {
                stack_push(stack, NEW_NODE(stack, i));
            }
            obs_test_eq(stack_length(stack), (int)100);
            stack_clear(stack);
            test_empty_list(stack, stack);
        })
    })

    OBS_REPORT
}