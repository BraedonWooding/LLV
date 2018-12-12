#ifndef LLV_HELPER_H
#define LLV_HELPER_H

/* A collection of helper functions */

#include <stdlib.h>
#include <stdbool.h>

#ifndef DEFAULT_UNICODE_SUPPORT
#define DEFAULT_UNICODE_SUPPORT (false)
#endif

#ifdef _WIN32
    #define WINDOWS_COMPATIBILITY
    #define CLEAR_SCREEN "cls"
#else
    #define UNIX_COMPATIBILITY
    #define CLEAR_SCREEN "clear"
#endif

#ifndef DEFAULT_TERMINAL_WIDTH
#define DEFAULT_TERMINAL_WIDTH (80)
#endif

#ifndef DEFAULT_TERMINAL_HEIGHT
#define DEFAULT_TERMINAL_HEIGHT (80)
#endif

typedef struct _terminal_size_t {
    int width;
    int height;
} terminalSize;

void write_str_to_buf(wchar_t **buf, size_t offset, size_t len, size_t index,
                      wchar_t *str, size_t str_len);

void write_str_center_of_buf(wchar_t **buf, size_t offset, size_t len,
                             wchar_t *str, size_t str_len);

void write_str_repeat_char(wchar_t *buf, size_t offset, wchar_t c, int count);

void write_str_repeat_char_vert(wchar_t **buf, size_t offset, wchar_t c, int count, int index);

void write_str_repeat_char_grid(wchar_t **buf, size_t offset, wchar_t c, int vert_count,
                                int horiz_count, int index);

terminalSize get_terminal_size(void);

/*
    Sleeps for the given amount of time.
*/
void sleep_ms(size_t ms);

/*
    Mallocs 'size' memory and exits with OOM message if memory is NULL.
*/
void *malloc_with_oom(size_t size, char *obj_name);

/*
    Returns true if the terminal supports unicode.
    Not necessarily always correct but I don't think it can be false positive.
    So we should be fine.
*/
bool supports_unicode(void);

/*
    Chooses 'unicode' string if unicode support is enabled else will choose 'backup'.
*/
wchar_t select_char_unicode(wchar_t unicode, wchar_t backup);

#endif
