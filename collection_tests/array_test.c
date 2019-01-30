#include "../include/collections/array.h"
#include "../lib/obsidian.h"
#include "../include/llv.h"
#include "collection_test_helper.h"
#include "../include/types/collection_skeleton.h"
#include "../include/types/shared_types.h"
#include <string.h>

int main(void) {
    OBS_SETUP("Array");

    OBS_TEST_GROUP("array_new/new_node", {
        OBS_TEST("Create list and test properties", {
            Array array = array_new("1", 0);
            obs_test_strcmp(array->parent.name, "1");
            obs_test(array_length(array), ==, (int)0);
            array_free(array);
        })

        OBS_TEST("Create node and test properties", {
            // NOTE: no point testing the other data members when selecting one
            // i.e. if we have set int_data = 4 we shouldn't test str_data
            // this is because it is classified as undefined behaviour and
            // we shouldn't expect undefined behaviour to behave in any way.

            // int node
            struct _array_data_t node = array_new_node((Data){.int_data = 4}, INTEGER);
            obs_test(node.data_tag, ==, INTEGER);
            obs_test(node.data.int_data, ==, (long long)4);

            // flt node
            node = array_new_node((Data){.flt_data = 5.9}, FLOAT);
            obs_test(node.data_tag, ==, FLOAT);
            obs_test(node.data.flt_data, ==, 5.9);

            // str node
            node = array_new_node((Data){.str_data = "Hello"}, STRING);
            obs_test(node.data_tag, ==, STRING);
            obs_test_strcmp(node.data.str_data, "Hello");

            // any node
            int x = 9;
            node = array_new_node((Data){.any_data = &x}, ANY);
            obs_test(node.data_tag, ==, ANY);
            obs_test((int*)node.data.any_data, ==, &x);
        })

        OBS_TEST("Create node using generic macros", {
            // int node
            struct _array_data_t node = NEW_NODE(array, 4);
            obs_test(node.data_tag, ==, INTEGER);
            obs_test(node.data.int_data, ==, (long long)4);

            // flt node
            node = NEW_NODE(array, 5.9);
            obs_test(node.data_tag, ==, FLOAT);
            obs_test(node.data.flt_data, ==, 5.9);

            // str node
            node = NEW_NODE(array, "Hello");
            obs_test(node.data_tag, ==, STRING);
            obs_test_strcmp(node.data.str_data, "Hello");

            // any node
            int x = 9;
            node = NEW_NODE(array, &x);
            obs_test(node.data_tag, ==, ANY);
            obs_test((int*)node.data.any_data, ==, &x);
        })
    });

    OBS_TEST_GROUP("array_at/set", {
        OBS_TEST("Setting and then overriding", {
            Array array = array_new("1", 5);
            for (int i = 0; i < 5; i++) {
                array_set(array, i, NEW_NODE(array, i));
                obs_test(array_at(array, i)->data.int_data, ==, i);
            }

            for (int i = 0; i < 5; i++) {
                obs_test(array_at(array, i)->data.int_data, ==, i);
                array_set(array, i, NEW_NODE(array, i * 10));
                obs_test(array_at(array, i)->data.int_data, ==, i * 10);
            }

            array_free(array);
        })
    })

    OBS_TEST_GROUP("array_resize", {
        OBS_TEST("resize empty", {
            Array array = array_new("1", 0);
            obs_test(array_length(array), ==, 0);
            for (int i = 1; i < 10; i++) {
                array_resize(array, i * 10);
                obs_test(array_length(array), ==, i * 10);
            }
            array_free(array);
        })
        OBS_TEST("resize with elements", {
            Array array = array_new("1", 0);
            obs_test(array_length(array), ==, 0);
            for (int i = 1; i < 10; i++) {
                array_resize(array, i * 10);
                obs_test(array_length(array), ==, i * 10);
                for (int j = 0; j < (i - 1) * 10; j++) {
                    obs_test(array_at(array, j)->data.int_data, ==, j);
                }
                for (int j = (i - 1) * 10; j < i * 10; j++) {
                    array_set(array, j, NEW_NODE(array, j));
                    obs_test(array_at(array, j)->data.int_data, ==, j);
                }
            }
            array_free(array);
        })
    })

    OBS_REPORT;
}