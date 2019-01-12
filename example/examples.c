// This is because when we are running this for tests we don't have it installed
// to our include path (or lib path) so we just use relative directs
#ifndef TESTING
#include <LLV/collections/ll.h>
#include <LLV/collections/dll.h>
#include <LLV/collections/array.h>
#include <LLV/llv.h>
#else
#include "../include/collections/ll.h"
#include "../include/collections/dll.h"
#include "../include/collections/array.h"
#include "../include/llv.h"
#endif

#include <locale.h>

#include <stdio.h>
#include <stdlib.h>

// Look at examples.incl for the code

#define ARRAY_LEN(a) (sizeof(a) / sizeof(*a))

void usage_exit(void) {
    printf("Use like; ./program <X> where X is a number and from the following list\n"
#define EXAMPLE_X(name, num, desc, func) #num ": " desc "\n"
#include "examples.incl"
#undef EXAMPLE_X
          );
    exit(1);
}

#define EXAMPLE_X(name, num, desc, func) void name(void) func
#include "examples.incl"
#undef EXAMPLE_X

int main(int argc, char *argv[]) {
    if (argc != 2) usage_exit();
    int num = atoi(argv[1]);

    // Cross platform clearing
    system(CLEAR_SCREEN);

    switch (num) {
#define EXAMPLE_X(name, num, desc, func) case num: { name(); } break;
#include "examples.incl"
#undef EXAMPLE_X
        default: usage_exit();
    }
}