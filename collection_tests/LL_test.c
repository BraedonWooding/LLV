#include "../include/collections/LL.h"
#include "../lib/obsidian.h"
#include "../include/llv.h"
#include <string.h>

// we use macros just because obs_test refers to variables
// that are defined in OBS_SETUP and so we need to be in scope

#define add_items_to_LL(list, len, data) \
    for (int i = 0; i < len; i++) { \
        LL_append(list, NEW_NODE(LL, data[i])); \
    } \

#define get_data(data, list) \
    _Generic((list[0]), \
        long: data.int_data, \
        long long: data.int_data, \
        unsigned int: data.int_data, \
        unsigned long: data.int_data, \
        unsigned long long: data.int_data, \
        int: data.int_data, \
        float: data.flt_data, \
        double: data.flt_data, \
        long double: data.flt_data, \
        char: data.int_data, \
        short: data.int_data, \
        unsigned short: data.int_data, \
        char *: data.str_data, \
        bool: data.int_data, \
        default: data.any_data \
    )

#define test_ll_strcmp(list, exp) { \
    int i = 0; \
    for (LL_Node n = list->head; n != NULL; n = n->next, i++) { \
        obs_test_strcmp(n->data.str_data, exp[i]) \
    } \
}

#define test_ll(list, exp) { \
    int i = 0; \
    for (LL_Node n = list->head; n != NULL; n = n->next, i++) { \
        obs_test(get_data(n->data, exp), ==, exp[i]) \
    } \
    obs_test(LL_length(list), ==, (size_t)i); \
}

#define test_empty(list) \
    obs_test(list->head, ==, NULL); \
    obs_test(list->tail, ==, NULL); \
    obs_test(LL_length(list), ==, (size_t)0); \
    obs_test_true(LL_is_empty(list)); \

int main(void) {
    OBS_SETUP("Linked List")

    OBS_TEST_GROUP("LL_new && LL_new_node", {
        OBS_TEST("Create list and test properties", {
            LL list = LL_new("1");
            obs_test_strcmp(list->name, "1");
            test_empty(list);
            LL_free(list);
        })

        OBS_TEST("Create node and test properties", {
            // NOTE: no point testing the other data members when selecting one
            // i.e. if we have set int_data = 4 we shouldn't test str_data
            // this is because it is classified as undefined behaviour and
            // we shouldn't expect undefined behaviour to behave in any way.

            // int node
            LL_Node node = LL_new_node((Data){.int_data = 4}, INTEGER);
            obs_test(node->data_tag, ==, INTEGER);
            obs_test(node->next, ==, NULL);
            obs_test(node->ptr, ==, NULL);
            obs_test(node->data.int_data, ==, (long long)4);
            LL_free_node(node);

            // flt node
            node = LL_new_node((Data){.flt_data = 5.9}, FLOAT);
            obs_test(node->data_tag, ==, FLOAT);
            obs_test(node->next, ==, NULL);
            obs_test(node->ptr, ==, NULL);
            obs_test(node->data.flt_data, ==, 5.9);
            LL_free_node(node);

            // str node
            node = LL_new_node((Data){.str_data = "Hello"}, STRING);
            obs_test(node->data_tag, ==, STRING);
            obs_test(node->next, ==, NULL);
            obs_test(node->ptr, ==, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            LL_free_node(node);

            // any node
            int x = 9;
            node = LL_new_node((Data){.any_data = &x}, ANY);
            obs_test(node->data_tag, ==, ANY);
            obs_test(node->next, ==, NULL);
            obs_test(node->ptr, ==, NULL);
            obs_test(node->data.any_data, ==, &x);
            LL_free_node(node);
        })

        OBS_TEST("Create node using generic macros", {
            // int node
            LL_Node node = NEW_NODE(LL, 4);
            obs_test(node->data_tag, ==, INTEGER);
            obs_test(node->next, ==, NULL);
            obs_test(node->ptr, ==, NULL);
            obs_test(node->data.int_data, ==, (long long)4);
            LL_free_node(node);

            // flt node
            node = NEW_NODE(LL, 5.9);
            obs_test(node->data_tag, ==, FLOAT);
            obs_test(node->next, ==, NULL);
            obs_test(node->ptr, ==, NULL);
            obs_test(node->data.flt_data, ==, 5.9);
            LL_free_node(node);

            // str node
            node = NEW_NODE(LL, "Hello");
            obs_test(node->data_tag, ==, STRING);
            obs_test(node->next, ==, NULL);
            obs_test(node->ptr, ==, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            LL_free_node(node);

            // any node
            int x = 9;
            node = NEW_NODE(LL, &x);
            obs_test(node->data_tag, ==, ANY);
            obs_test(node->next, ==, NULL);
            obs_test(node->ptr, ==, NULL);
            obs_test(node->data.any_data, ==, &x);
            LL_free_node(node);
        })
    });

    OBS_TEST_GROUP("LL_clear_list", {
        OBS_TEST("Clearing empty list", {
            LL list = LL_new("1");
            test_empty(list);
            LL_clear_list(list);
            test_empty(list);
            LL_free(list);
        })

        OBS_TEST("Clearing list with elements", {
            LL list = LL_new("2");
            long long elements[5] = ((long long[]){1, 2, 3, 4, 5});
            add_items_to_LL(list, 5, elements);
            test_ll(list, elements);
            LL_clear_list(list);
            test_empty(list);
            LL_free(list);
        })

        OBS_TEST("Clearing list then adding different elements", {
            LL list = LL_new("3");
            double *elements = ((double[]){5.0, 3.2, 100.5, 421.222, 1.022, 2.22, 0.2});
            add_items_to_LL(list, 7, elements);
            test_ll(list, elements);
            LL_clear_list(list);
            test_empty(list);
            char *new_elements[3] = ((char*[]){"Hello", "World", "Wow"});
            add_items_to_LL(list, 3, new_elements);
            test_ll_strcmp(list, new_elements);
            LL_clear_list(list);
            test_empty(list);
            LL_free(list);
        })
    })

    OBS_TEST_GROUP("LL_insert_after/before", {
        OBS_TEST("Inserting after on empty list", {
            LL list = LL_new("1");
            test_empty(list);
            LL_insert_after(list, NEW_NODE(LL, 5), list->head);
            test_ll(list, (long long[]){5});
            LL_free(list);
        })

        OBS_TEST("Inserting before on empty list", {
            LL list = LL_new("2");
            test_empty(list);
            LL_insert_before(list, NEW_NODE(LL, "hello"), list->head);
            test_ll_strcmp(list, (char*[]){"hello"});
            LL_free(list);
        })

        OBS_TEST("Inserting after on single element list", {
            LL list = LL_new("3");
            LL_insert_after(list, NEW_NODE(LL, 5.4), list->head);
            test_ll(list, ((double[]){5.4}));
            LL_insert_after(list, NEW_NODE(LL, 2.2), list->head);
            test_ll(list, ((double[]){5.4, 2.2}));
            LL_free(list);
        })

        OBS_TEST("Inserting before on single element list", {
            LL list = LL_new("4");
            LL_insert_before(list, NEW_NODE(LL, 5.4), list->head);
            test_ll(list, ((double[]){5.4}));
            LL_insert_before(list, NEW_NODE(LL, 2.2), list->head);
            test_ll(list, ((double[]){2.2, 5.4}));
            LL_free(list);
        })

        OBS_TEST("Inserting after in middle of list", {
            LL list = LL_new("5");
            long long items[10] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            LL_insert_after(list, NEW_NODE(LL, 100), list->head->next->next->next);
            long long result[11] = ((long long[]){1, 2, 3, 4, 100, 5, 6, 7, 8, 9, 10});
            test_ll(list, result);
            LL_free(list);
        })

        OBS_TEST("Inserting after on last element", {
            LL list = LL_new("6");
            long long items[10] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            LL_insert_after(list, NEW_NODE(LL, 100), list->tail);
            long long result[11] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100});
            test_ll(list, result);
            LL_free(list);
        })

        OBS_TEST("Inserting before in middle of list", {
            LL list = LL_new("7");
            long long items[10] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            LL_insert_before(list, NEW_NODE(LL, 100), list->head->next->next->next);
            long long result[11] = ((long long[]){1, 2, 3, 100, 4, 5, 6, 7, 8, 9, 10});
            test_ll(list, result);
            LL_free(list);
        })

        OBS_TEST("Inserting before on first element", {
            LL list = LL_new("8");
            long long items[10] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            LL_insert_before(list, NEW_NODE(LL, 100), list->head);
            long long result[11] = ((long long[]){100, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            test_ll(list, result);
            LL_free(list);
        })
    })

    OBS_TEST_GROUP("LL_remove_node", {
        OBS_TEST("Remove a NULL node from an empty list", {
            LL list = LL_new("1");
            obs_assert(LL_remove_node(list, NULL), ==, NULL);
            LL_free(list);
        })

        OBS_TEST("Remove a non null node from an empty list", {
            LL list = LL_new("1");
            LL_Node n = NEW_NODE(LL, 5);
            obs_assert(LL_remove_node(list, n), ==, NULL);
            LL_free_node(n);
            LL_free(list);
        })

        OBS_TEST("Remove a node from an non empty list", {
            LL list = LL_new("3");
            long long items[10] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            LL_remove_node(list, list->head->next->next);
            long long result[9] = ((long long[]){1, 2, 4, 5, 6, 7, 8, 9, 10});
            test_ll(list, result);
            LL_free(list);
        })

        OBS_TEST("Remove the only node from an non empty list", {
            LL list = LL_new("4");
            LL_Node original = NEW_NODE(LL, 1);
            LL_insert_after(list, original, list->head);
            LL_Node n = LL_remove_node(list, list->head);
            test_empty(list);
            obs_test(original, ==, n);
            LL_free(list);
            LL_free_node(n);
        })

        OBS_TEST("Remove first and last then the rest (from forward to back)", {
            LL list = LL_new("5");
            long long items[10] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            // remove front
            LL_free_node(LL_remove_node(list, list->head));
            long long result1[9] = ((long long[]){2, 3, 4, 5, 6, 7, 8, 9, 10});
            test_ll(list, result1);
            // remove back
            LL_free_node(LL_remove_node(list, list->tail));
            long long result2[8] = ((long long[]){2, 3, 4, 5, 6, 7, 8, 9});
            test_ll(list, result2);
            // remove rest
            while (!LL_is_empty(list)) {
                LL_free_node(LL_remove_node(list, list->head));
            }
            test_empty(list);
            LL_free(list);
        })
    })

    OBS_TEST_GROUP("LL_is_empty", {
        OBS_TEST("Empty list", {
            LL list = LL_new("1");
            obs_test_true(LL_is_empty(list));
            LL_free(list);
        })

        OBS_TEST("Single element list", {
            LL list = LL_new("2");
            LL_append(list, NEW_NODE(LL, 2));
            obs_test_false(LL_is_empty(list));
            LL_free(list);
        })

        OBS_TEST("Multi element list", {
            LL list = LL_new("3");
            long long items[10] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            obs_test_false(LL_is_empty(list));
            LL_free(list);
        })
    })

    OBS_TEST_GROUP("LL_find_prev/next", {

    })

    OBS_TEST_GROUP("LL_length", {
        OBS_TEST("Empty list", {
            LL list = LL_new("1");
            obs_test(LL_length(list), ==, (size_t)0);
            LL_free(list);
        })

        OBS_TEST("Single element list", {
            LL list = LL_new("2");
            LL_push(list, NEW_NODE(LL, 2));
            obs_test(LL_length(list), ==, (size_t)1);
            LL_free(list);
        })

        OBS_TEST("Multi element list", {
            LL list = LL_new("3");
            long long items[10] = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            add_items_to_LL(list, 10, items);
            test_ll(list, items);
            obs_test(LL_length(list), ==, (size_t)10);
            LL_free(list);
        })
    })

    OBS_TEST_GROUP("LL_push/pop", {
        
    })

    OBS_TEST_GROUP("LL_append", {

    })

    OBS_REPORT
}
