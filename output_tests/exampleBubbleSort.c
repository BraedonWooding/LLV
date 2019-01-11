#include <LLV/collections/array.h>
#include <LLV/llv.h>
#include <LLV/helper.h>

int main(void) {
    Array array = array_new("to sort", 10);
    int *values = ((int[]){1000, 24, 12, 2, 9, 90, 110, 54, 1, 10});
    for (int i = 0; i < 10; i++) {
        array_set(array, i, NEW_NODE(array, values[i]));
    }

    fmt_update("%l", array);

    ArrayNode n1 = NULL;
    ArrayNode n2 = NULL;
    attach_ptr(&n1, "a");
    attach_ptr(&n2, "b");

    bool changes = true;
    while (changes) {
        changes = false;
        for (int i = 0; i < 9; i++) {
            n1 = array_at(array, i);
            n2 = array_at(array, i + 1);
            fmt_update("%l", array);

            // swap
            if (n1->data.int_data > n2->data.int_data) {
                struct _array_data_t tmp = *n1;
                array_set(array, i, *n2);
                array_set(array, i + 1, tmp);
                changes = true;

                n2 = n1;
                n1 = array_at(array, i + 1);
                fmt_update("%l %s", array, "b < a therefore swap");
            }
        }
    }

    deattach_ptr(&n1, "a");
    deattach_ptr(&n2, "b");
    fmt_update("%s %l", "All sorted", array);
    array_free(array);
}
