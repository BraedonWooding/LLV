#ifndef LLV_HELPER_H
#define LLV_HELPER_H

/* A collection of helper functions */

#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#   define WINDOWS_COMPATIBILITY
#   define CLEAR_SCREEN "cls"
#else
#   define UNIX_COMPATIBILITY
#   define CLEAR_SCREEN "clear"
#endif

typedef struct _terminal_int {
    int width;
    int height;
} terminalSize;

void write_str_to_buf(wchar_t **buf, int offset, int len, int index,
                      wchar_t *str, int str_len);

void write_str_center_of_buf(wchar_t **buf, int offset, int len,
                             wchar_t *str, int str_len);

void write_str_repeat_char(wchar_t *buf, int offset, wchar_t c, int count);

void write_str_repeat_char_vert(wchar_t **buf, int offset, wchar_t c, int count, int index);

void write_str_repeat_char_grid(wchar_t **buf, int offset, wchar_t c, int vert_count,
                                int horiz_count, int index);

terminalSize get_terminal_size(void);

/*
    Sleeps for the given amount of time.
*/
void sleep_ms(int ms);

/*
    Mallocs 'size' memory and exits with OOM message if memory is NULL.
*/
void *malloc_with_oom(size_t size, char *obj_name);

bool contains_utf(char *str);

/*
    Chooses 'unicode' string if unicode support is enabled else will choose 'backup'.
*/
wchar_t select_char_unicode(wchar_t unicode, wchar_t backup);

wchar_t *select_str_unicode(wchar_t *unicode, wchar_t *backup);

bool str_icase_eql(char *a, char *b);
bool atob(char *str);
bool supports_unicode(void);

void assert_msg(bool expr, char *fmt, ...);

#endif /* LLV_HELPER_H */
