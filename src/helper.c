#include "../include/helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#ifdef WINDOWS_COMPATIBILITY
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>

    // https://stackoverflow.com/questions/1157209/is-there-an-alternative-sleep-function-in-c-to-milliseconds
    #if _POSIX_C_SOURCE >= 199309L
        #include <time.h>
    #else
        #include <unistd.h>
        #define POSIX_LEGACY
    #endif
#endif

extern bool disable_unicode;

void write_str_center_of_buf(wchar_t **buf, size_t offset, size_t len,
                             wchar_t *str, size_t str_len) {
    write_str_to_buf(buf, offset, len, len / 2, str, str_len);
}

void write_str_to_buf(wchar_t **buf, size_t offset, size_t len, size_t index,
                      wchar_t *str, size_t str_len) {
    memcpy(buf[index] + offset, str, str_len * sizeof(wchar_t));
}

terminalSize get_terminal_size(void) {
#ifndef TESTING
    int cols, rows;
    #ifdef WINDOWS_COMPATIBILITY
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        cols = w.ws_col;
        rows = w.ws_row;
    #endif

    // this effects debuggers/tests
    if (cols == 0) {
        cols = DEFAULT_TERMINAL_WIDTH;
        rows = DEFAULT_TERMINAL_HEIGHT;
    }

    return (terminalSize){.width = cols, .height = rows};
#else
    return (terminalSize){.width = DEFAULT_TERMINAL_WIDTH, .height = DEFAULT_TERMINAL_HEIGHT};
#endif
}

void sleep_ms(size_t ms) {
    #ifdef WINDOWS_COMPATIBILITY
        Sleep(ms);
    #elif defined(POSIX_LEGACY)
        // use timeout as opposed to usleep
        struct timeval tv = { .tv_sec = ms / 1000, .tv_usec = (ms % 1000) * 1000 };
        select(0, NULL, NULL, NULL, &tv);
    #else
        struct timespec ts = { .tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000 };
        nanosleep(&ts, NULL);
    #endif
}

/*
    @Refactor: maybe could include `__FILE__:__LINE__` instead of the obj_name??
               would have to make this a macro in that case (to get the file/line right)
*/
void *malloc_with_oom(size_t size, char *obj_name) {
    void *obj = malloc(size);
    if (obj == NULL) {
        printf("Error: OOM; can't allocate %lu bytes for %s\n", size, obj_name);
        exit(1);
    }
    return obj;
}

void write_str_repeat_char(wchar_t *buf, size_t offset, wchar_t c, int count) {
    for (int i = 0; i < count; i++) buf[i + offset] = c;
}

void write_str_repeat_char_vert(wchar_t **buf, size_t offset, wchar_t c, int count, int index) {
    for (int i = 0; i < count; i++) buf[i][index + offset] = c;
}

void write_str_repeat_char_grid(wchar_t **buf, size_t offset, wchar_t c, int vert_count,
                                int horiz_count, int index) {
    for (int i = 0; i < horiz_count; i++) {
        write_str_repeat_char_vert(buf, offset, ' ', vert_count, i + index);
    }
}

// taken (and modified) from: https://rosettacode.org/wiki/Terminal_control/Unicode_output#C
bool supports_unicode(void) {
    if (disable_unicode) return false;

    // cache value, no need to generate twice
    static int res = -1;
    if (res != -1) return res;

    char *str = getenv("LANG");
    puts(str);

    // just looks for 'utf'
    // not necessarily always true but I haven't seen a case of a false positive
    // just false negatives so for the default value of false it won't ever cause issues
    for (int i = 0; str[i] != '\0' && str[i + 1] != '\0' && str[i + 2] != '\0'; i++) {
        if (tolower(str[i]) == 'u' && tolower(str[i + 1]) == 't' &&
            tolower(str[i + 2]) == 'f') {
            res = true;
            printf("HERE\n");
            #ifdef UNIX_COMPATIBILITY
            // @Weird: for some reason we need to set the locale on unix systems
            // (not all just high sierra and ubuntu so far) otherwise nothing is printed.
            setlocale(LC_ALL, "");
            #endif
            return res;
        }
    }

    res = DEFAULT_UNICODE_SUPPORT;
    return res;
}

wchar_t select_char_unicode(wchar_t unicode, wchar_t backup) {
    return supports_unicode() ? unicode : backup;
}
