#include "../include/collections/list.h"
#include "../lib/obsidian.h"
#include "../include/llv.h"
#include "collection_test_helper.h"
#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"
#include <string.h>

#define dynlist_test(arr, list) \
<<<<<<< HEAD
    for (int i = 0; i < list_length(list); i++) { \
=======
    for (size_t i = 0; i < list_length(list); i++) { \
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
        obs_test(get_data(list->data[i].data, arr), ==, arr[i]) \
    } \

int main(void) {
    OBS_SETUP("Growable List")

    OBS_TEST_GROUP("List_new && list_new_node", {
        OBS_TEST("Create list and test properties", {
            List list = list_new("1");
            obs_test_strcmp(list->parent.name, "1");
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)0);
=======
            obs_test(list_length(list), ==, (size_t)0);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            obs_test_true(list_is_empty(list));
            list_free(list);
        })

        OBS_TEST("Create node and test properties", {
            // NOTE: no point testing the other data members when selecting one
            // i.e. if we have set int_data = 4 we shouldn't test str_data
            // this is because it is classified as undefined behaviour and
            // we shouldn't expect undefined behaviour to behave in any way.

            // int node
            struct _list_data_t node = list_new_node((Data){.int_data = 4}, INTEGER);
            obs_test(node.data_tag, ==, INTEGER);
            obs_test(node.data.int_data, ==, (long long)4);

            // flt node
            node = list_new_node((Data){.flt_data = 5.9}, FLOAT);
            obs_test(node.data_tag, ==, FLOAT);
            obs_test(node.data.flt_data, ==, 5.9);

            // str node
            node = list_new_node((Data){.str_data = "Hello"}, STRING);
            obs_test(node.data_tag, ==, STRING);
            obs_test_strcmp(node.data.str_data, "Hello");

            // any node
            int x = 9;
            node = list_new_node((Data){.any_data = &x}, ANY);
            obs_test(node.data_tag, ==, ANY);
            obs_test((int*)node.data.any_data, ==, &x);
        })

        OBS_TEST("Create node using generic macros", {
            // int node
            struct _list_data_t node = NEW_NODE(list, 4);
            obs_test(node.data_tag, ==, INTEGER);
            obs_test(node.data.int_data, ==, (long long)4);

            // flt node
            node = NEW_NODE(list, 5.9);
            obs_test(node.data_tag, ==, FLOAT);
            obs_test(node.data.flt_data, ==, 5.9);

            // str node
            node = NEW_NODE(list, "Hello");
            obs_test(node.data_tag, ==, STRING);
            obs_test_strcmp(node.data.str_data, "Hello");

            // any node
            int x = 9;
            node = NEW_NODE(list, &x);
            obs_test(node.data_tag, ==, ANY);
            obs_test((int*)node.data.any_data, ==, &x);
        })
    });

    OBS_TEST_GROUP("List_clear", {
        OBS_TEST("Clearing empty list", {
            List list = list_new("1");
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)0);
            obs_test_true(list_is_empty(list));
            list_clear(list, false);
            obs_test(list_length(list), ==, (int)0);
=======
            obs_test(list_length(list), ==, (size_t)0);
            obs_test_true(list_is_empty(list));
            list_clear(list, false);
            obs_test(list_length(list), ==, (size_t)0);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            obs_test_true(list_is_empty(list));
            list_free(list);
        })

        OBS_TEST("Clearing list with elements", {
            List list = list_new("2");
            long long *elements = ((long long[]){1, 2, 3, 4, 5});
            map_items(list, 5, elements, list, list_push_back);
            dynlist_test(elements, list);
            list_clear(list, false);
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)0);
=======
            obs_test(list_length(list), ==, (size_t)0);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            obs_test_true(list_is_empty(list));
            list_free(list);
        })

        OBS_TEST("Clearing list then adding different elements", {
            List list = list_new("3");
            list_free(list);
        })
    })

    OBS_TEST_GROUP("List_insert_after/before", {
        OBS_TEST("Inserting after on empty list", {
            List list = list_new("1");
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)0);
=======
            obs_test(list_length(list), ==, (size_t)0);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            obs_test_true(list_is_empty(list));
            list_free(list);
        })

        OBS_TEST("Inserting before on empty list", {
            List list = list_new("2");
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)0);
=======
            obs_test(list_length(list), ==, (size_t)0);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            obs_test_true(list_is_empty(list));
            list_free(list);
        })

        OBS_TEST("Inserting after on single element list", {
            List list = list_new("3");
            list_free(list);
        })

        OBS_TEST("Inserting before on single element list", {
            List list = list_new("4");
            list_free(list);
        })

        OBS_TEST("Inserting after in middle of list", {
            List list = list_new("5");
            list_free(list);
        })

        OBS_TEST("Inserting after on last element", {
            List list = list_new("6");
            list_free(list);
        })

        OBS_TEST("Inserting before in middle of list", {
            List list = list_new("7");
            list_free(list);
        })

        OBS_TEST("Inserting before on first element", {
            List list = list_new("8");
            list_free(list);
        })
    })

    OBS_TEST_GROUP("list_remove", {
    })

    OBS_TEST_GROUP("list_is_empty", {
        OBS_TEST("Empty list", {
            List list = list_new("1");
            obs_test_true(list_is_empty(list));
            list_free(list);
        })

        OBS_TEST("Single element list", {
            List list = list_new("2");
            list_push_back(list, NEW_NODE(list, 2));
            obs_test_false(list_is_empty(list));
            list_free(list);
        })

        OBS_TEST("Multi element list", {
            List list = list_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, list, list_push_back);
            dynlist_test(items, list);
            obs_test_false(list_is_empty(list));
            list_free(list);
        })
    })

    OBS_TEST_GROUP("list_length", {
        OBS_TEST("Empty list", {
            List list = list_new("1");
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)0);
=======
            obs_test(list_length(list), ==, (size_t)0);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            list_free(list);
        })

        OBS_TEST("Single element list", {
            List list = list_new("2");
            list_push_back(list, NEW_NODE(list, 2));
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)1);
=======
            obs_test(list_length(list), ==, (size_t)1);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            list_free(list);
        })

        OBS_TEST("Multi element list", {
            List list = list_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, list, list_push_back);
            dynlist_test(items, list);
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)10);
=======
            obs_test(list_length(list), ==, (size_t)10);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            list_free(list);
        })
    })

    OBS_TEST_GROUP("List_push_back", {
        OBS_TEST("Append to empty list", {
            List list = list_new("1");
            list_push_back(list, NEW_NODE(list, 10));
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)1);
=======
            obs_test(list_length(list), ==, (size_t)1);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            obs_test(list->data[0].data.int_data, ==, (long long)10);
            list_free(list);
        })

        OBS_TEST("Append to non-empty list", {
            List list = list_new("1");
            long long *elements = ((long long[]){1, 2, 5, 9, 2, 4, 5});
            map_items(list, 7, elements, list, list_push_back);
            dynlist_test(elements, list);
            list_push_back(list, NEW_NODE(list, 10));
<<<<<<< HEAD
            obs_test(list_length(list), ==, (int)8);
=======
            obs_test(list_length(list), ==, (size_t)8);
>>>>>>> 8d307e6463580fea58edac8c55a5ae155aa5c6ca
            obs_test(list->data[list->cur_len - 1].data.int_data, ==, (long long)10);
            list_free(list);
        })
    })

    OBS_REPORT
}
