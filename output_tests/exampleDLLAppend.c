#include <LLV/collections/dll.h>
#include <LLV/llv.h>
#include <LLV/helper.h>

int main(void) {
    DLL list = dll_new("list");
    fmt_update("%l", list);
    int *values = ((int[]){100, 24, 345, 59, 393, 292, 12});
    for (int i = 0; i < 7; i++) {
        dll_append(list, NEW_NODE(dll, values[i]));
        fmt_update("%l", list);
    }
    fmt_update("%l", list);
}
