#include <LLV/collections/dll.h>
#include <LLV/collections/ll.h>
#include "../lib/obsidian.h"
#include <LLV/llv.h>
#include "collection_test_helper.h"
#include <LLV/types/collection_skeleton.h>
#include <LLV/types/shared_types.h>
#include <string.h>

/*
    Practically identical tests to LL though I'm sure I'll expand it later.
*/

int main(void) {
    OBS_SETUP("Doubly Linked List")

    OBS_TEST_GROUP("DLL_new && dll_new_node", {
        OBS_TEST("Create list and test properties", {
            DLL list = dll_new("1");
            obs_test_strcmp(list->name, "1");
            test_empty_list(list, dll);
            dll_free(list);
        })

        OBS_TEST("Create node and test properties", {
            // NOTE: no point testing the other data members when selecting one
            // i.e. if we have set int_data = 4 we shouldn't test str_data
            // this is because it is classified as undefined behaviour and
            // we shouldn't expect undefined behaviour to behave in any way.

            // int node
            DLL_Node node = dll_new_node((Data){.int_data = 4}, INTEGER);
            obs_test(node->data_tag, ==, INTEGER);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.int_data, ==, (long long)4);
            dll_free_node(node);

            // flt node
            node = dll_new_node((Data){.flt_data = 5.9}, FLOAT);
            obs_test(node->data_tag, ==, FLOAT);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.flt_data, ==, 5.9);
            dll_free_node(node);

            // str node
            node = dll_new_node((Data){.str_data = "Hello"}, STRING);
            obs_test(node->data_tag, ==, STRING);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            dll_free_node(node);

            // any node
            int x = 9;
            node = dll_new_node((Data){.any_data = &x}, ANY);
            obs_test(node->data_tag, ==, ANY);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test((int*)node->data.any_data, ==, &x);
            dll_free_node(node);
        })

        OBS_TEST("Create node using generic macros", {
            // int node
            DLL_Node node = NEW_NODE(dll, 4);
            obs_test(node->data_tag, ==, INTEGER);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.int_data, ==, (long long)4);
            dll_free_node(node);

            // flt node
            node = NEW_NODE(dll, 5.9);
            obs_test(node->data_tag, ==, FLOAT);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test(node->data.flt_data, ==, 5.9);
            dll_free_node(node);

            // str node
            node = NEW_NODE(dll, "Hello");
            obs_test(node->data_tag, ==, STRING);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test_strcmp(node->data.str_data, "Hello");
            dll_free_node(node);

            // any node
            int x = 9;
            node = NEW_NODE(dll, &x);
            obs_test(node->data_tag, ==, ANY);
            obs_test((void*)node->next, ==, NULL);
            obs_test((void*)node->ptr, ==, NULL);
            obs_test((int*)node->data.any_data, ==, &x);
            dll_free_node(node);
        })
    });

    OBS_TEST_GROUP("DLL_clear_list", {
        OBS_TEST("Clearing empty list", {
            DLL list = dll_new("1");
            test_empty_list(list, dll);
            dll_clear_list(list);
            test_empty_list(list, dll);
            dll_free(list);
        })

        OBS_TEST("Clearing list with elements", {
            DLL list = dll_new("2");
            long long *elements = ((long long[]){1, 2, 3, 4, 5});
            map_items(list, 5, elements, dll, dll_append);
            test_list(list, elements, dll);
            dll_clear_list(list);
            test_empty_list(list, dll);
            dll_free(list);
        })

        OBS_TEST("Clearing list then adding different elements", {
            DLL list = dll_new("3");
            double *elements = ((double[]){5.0, 3.2, 100.5, 421.222, 1.022, 2.22, 0.2});
            map_items(list, 7, elements, dll, dll_append);
            test_list(list, elements, dll);
            dll_clear_list(list);
            test_empty_list(list, dll);
            char **new_elements = ((char*[]){"Hello", "World", "Wow"});
            map_items(list, 3, new_elements, dll, dll_append);
            test_strcmp_fakenode(list, new_elements);
            dll_clear_list(list);
            test_empty_list(list, dll);
            dll_free(list);
        })
    })

    OBS_TEST_GROUP("DLL_insert_after/before", {
        OBS_TEST("Inserting after on empty list", {
            DLL list = dll_new("1");
            test_empty_list(list, dll);
            dll_insert_after(list, NEW_NODE(dll, 5), list->head);
            test_list(list, (long long[]){5}, dll);
            dll_free(list);
        })

        OBS_TEST("Inserting before on empty list", {
            DLL list = dll_new("2");
            test_empty_list(list, dll);
            dll_insert_before(list, NEW_NODE(dll, "hello"), list->head);
            test_strcmp_fakenode(list, (char*[]){"hello"});
            dll_free(list);
        })

        OBS_TEST("Inserting after on single element list", {
            DLL list = dll_new("3");
            dll_insert_after(list, NEW_NODE(dll, 5.4), list->head);
            test_list(list, ((double[]){5.4}), dll);
            dll_insert_after(list, NEW_NODE(dll, 2.2), list->head);
            test_list(list, ((double[]){5.4, 2.2}), dll);
            dll_free(list);
        })

        OBS_TEST("Inserting before on single element list", {
            DLL list = dll_new("4");
            dll_insert_before(list, NEW_NODE(dll, 5.4), list->head);
            test_list(list, ((double[]){5.4}), dll);
            dll_insert_before(list, NEW_NODE(dll, 2.2), list->head);
            test_list(list, ((double[]){2.2, 5.4}), dll);
            dll_free(list);
        })

        OBS_TEST("Inserting after in middle of list", {
            DLL list = dll_new("5");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            dll_insert_after(list, NEW_NODE(dll, 100), list->head->next->next->next);
            long long *result = ((long long[]){1, 2, 3, 4, 100, 5, 6, 7, 8, 9, 10});
            test_list(list, result, dll);
            dll_free(list);
        })

        OBS_TEST("Inserting after on last element", {
            DLL list = dll_new("6");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            dll_insert_after(list, NEW_NODE(dll, 100), list->tail);
            long long *result = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100});
            test_list(list, result, dll);
            dll_free(list);
        })

        OBS_TEST("Inserting before in middle of list", {
            DLL list = dll_new("7");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            dll_insert_before(list, NEW_NODE(dll, 100), list->head->next->next->next);
            long long *result = ((long long[]){1, 2, 3, 100, 4, 5, 6, 7, 8, 9, 10});
            test_list(list, result, dll);
            dll_free(list);
        })

        OBS_TEST("Inserting before on first element", {
            DLL list = dll_new("8");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            dll_insert_before(list, NEW_NODE(dll, 100), list->head);
            long long *result = ((long long[]){100, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            test_list(list, result, dll);
            dll_free(list);
        })
    })

    OBS_TEST_GROUP("dll_remove_node", {
        OBS_TEST("Remove a NUDLL node from an empty list", {
            DLL list = dll_new("1");
            obs_assert(dll_remove_node(list, NULL), ==, NULL);
            dll_free(list);
        })

        OBS_TEST("Remove a non null node from an empty list", {
            DLL list = dll_new("1");
            DLL_Node n = NEW_NODE(dll, 5);
            obs_assert(dll_remove_node(list, n), ==, NULL);
            dll_free_node(n);
            dll_free(list);
        })

        OBS_TEST("Remove a node from an non empty list", {
            DLL list = dll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            dll_remove_node(list, list->head->next->next);
            long long *result = ((long long[]){1, 2, 4, 5, 6, 7, 8, 9, 10});
            test_list(list, result, dll);
            dll_free(list);
        })

        OBS_TEST("Remove the only node from an non empty list", {
            DLL list = dll_new("4");
            DLL_Node original = NEW_NODE(dll, 1);
            dll_insert_after(list, original, list->head);
            DLL_Node n = dll_remove_node(list, list->head);
            test_empty_list(list, dll);
            obs_test(original, ==, n);
            dll_free(list);
            dll_free_node(n);
        })

        OBS_TEST("Remove first and last then the rest (from forward to back)", {
            DLL list = dll_new("5");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            // remove front
            dll_free_node(dll_remove_node(list, list->head));
            long long *result1 = ((long long[]){2, 3, 4, 5, 6, 7, 8, 9, 10});
            test_list(list, result1, dll);
            // remove back
            dll_free_node(dll_remove_node(list, list->tail));
            long long *result2 = ((long long[]){2, 3, 4, 5, 6, 7, 8, 9});
            test_list(list, result2, dll);
            // remove rest
            while (!dll_is_empty(list)) {
                dll_free_node(dll_remove_node(list, list->head));
            }
            test_empty_list(list, dll);
            dll_free(list);
        })
    })

    OBS_TEST_GROUP("dll_is_empty", {
        OBS_TEST("Empty list", {
            DLL list = dll_new("1");
            obs_test_true(dll_is_empty(list));
            dll_free(list);
        })

        OBS_TEST("Single element list", {
            DLL list = dll_new("2");
            dll_append(list, NEW_NODE(dll, 2));
            obs_test_false(dll_is_empty(list));
            dll_free(list);
        })

        OBS_TEST("Multi element list", {
            DLL list = dll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            obs_test_false(dll_is_empty(list));
            dll_free(list);
        })
    })

    OBS_TEST_GROUP("DLL_find_prev/next", {
        OBS_TEST("Previous/Next of first element", {
            DLL list = dll_new("1");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            obs_test(dll_find_prev(list->head), ==, NULL);
            obs_test(dll_find_next(list->head)->data.int_data, ==, (long long)2);
            dll_free(list);
        })

        OBS_TEST("Previous/Next of last element", {
            DLL list = dll_new("2");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            obs_test(dll_find_prev(list->tail)->data.int_data, ==, (long long)9);
            obs_test(dll_find_next(list->tail), ==, NULL);
            dll_free(list);
        })

        OBS_TEST("Previous/Next of null element", {
            DLL list = dll_new("3");
            obs_test(dll_find_prev(NULL), ==, NULL);
            obs_test(dll_find_next(NULL), ==, NULL);
            dll_free(list);
        })

        OBS_TEST("Previous/Next of middle elements", {
            DLL list = dll_new("4");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            obs_test(dll_find_prev(list->head->next->next), ==, list->head->next);
            obs_test(dll_find_next(list->head->next->next->next), ==, list->head->next->next->next->next);
            dll_free(list);
        })
    })

    OBS_TEST_GROUP("dll_length", {
        OBS_TEST("Empty list", {
            DLL list = dll_new("1");
            obs_test(dll_length(list), ==, (size_t)0);
            dll_free(list);
        })

        OBS_TEST("Single element list", {
            DLL list = dll_new("2");
            dll_push(list, NEW_NODE(dll, 2));
            obs_test(dll_length(list), ==, (size_t)1);
            dll_free(list);
        })

        OBS_TEST("Multi element list", {
            DLL list = dll_new("3");
            long long *items = ((long long[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
            map_items(list, 10, items, dll, dll_append);
            test_list(list, items, dll);
            obs_test(dll_length(list), ==, (size_t)10);
            dll_free(list);
        })
    })

    OBS_TEST_GROUP("DLL_push/pop", {
        OBS_TEST("Push onto empty list, then pop it", {
            DLL list = dll_new("1");
            dll_push(list, NEW_NODE(dll, 4));
            obs_test(dll_length(list), ==, (size_t)1);
            obs_test(list->head->data.int_data, ==, (long long)4);
            DLL_Node n = dll_pop(list);
            obs_test(n->data.int_data, ==, (long long)4);
            dll_free_node(n);
            obs_test(dll_length(list), ==, (size_t)0);
            dll_free(list);
        })

        OBS_TEST("Pop empty list", {
            DLL list = dll_new("2");
            obs_test(dll_pop(list), ==, NULL);
            dll_free(list);
        })

        OBS_TEST("Push multiple then pop them", {
            long long *elements = ((long long[]){1, 2, 5, 9, 2, 4, 5});
            long long *result = ((long long[]){5, 4, 2, 9, 5, 2, 1});
            DLL list = dll_new("3");
            for (int i = 0; i < 7; i++) {
                dll_push(list, NEW_NODE(dll, elements[i]));
            }
            test_list(list, result, dll);
            for (int i = 0; i < 7; i++) {
                DLL_Node n = dll_pop(list);
                obs_test(n->data.int_data, ==, result[i]);
                dll_free_node(n);
            }
            obs_test(dll_length(list), ==, (size_t)0);
            dll_free(list);
        })
    })

    OBS_TEST_GROUP("DLL_append", {
        OBS_TEST("Append to empty list", {
            DLL list = dll_new("1");
            dll_append(list, NEW_NODE(dll, 10));
            obs_test(dll_length(list), ==, (size_t)1);
            obs_test(list->head->data.int_data, ==, (long long)10);
            dll_free(list);
        })

        OBS_TEST("Append to non-empty list", {
            DLL list = dll_new("1");
            long long *elements = ((long long[]){1, 2, 5, 9, 2, 4, 5});
            map_items(list, 7, elements, dll, dll_append);
            test_list(list, elements, dll);
            dll_append(list, NEW_NODE(dll, 10));
            obs_test(dll_length(list), ==, (size_t)8);
            obs_test(list->tail->data.int_data, ==, (long long)10);
            dll_free(list);
        })
    })

    OBS_REPORT
}
