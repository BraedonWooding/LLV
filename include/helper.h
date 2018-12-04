#ifndef LLV_HELPER_H
#define LLV_HELPER_H

/* A collection of helper functions */

#include <stdlib.h>

#ifdef _WIN32
    #define WINDOWS_COMPATIBILITY
    #define CLEAR_SCREEN "cls"
#else
    #define UNIX_COMPATIBILITY
    #define CLEAR_SCREEN "clear"
#endif

typedef struct _terminal_size_t {
    int width;
    int height;
} terminalSize;

void write_str_to_buf(char **buf, size_t offset, size_t len, size_t index,
                      char *str, size_t str_len);

void write_str_center_of_buf(char **buf, size_t offset, size_t len,
                             char *str, size_t str_len);

void write_str_repeat_char(char *buf, size_t offset, char c, int count);

void write_str_repeat_char_vert(char **buf, size_t offset, char c, int count, int index);

terminalSize get_terminal_size(void);

/*
    Sleeps for the given amount of time.
*/
void sleep_ms(size_t ms);

/*
    Mallocs 'size' memory and exits with OOM message if memory is NULL.
*/
void *malloc_with_oom(size_t size, char *obj_name);

#endif
