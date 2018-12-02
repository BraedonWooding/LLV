#include "helper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void write_str_center_of_buf(char **buf, size_t offset, size_t len,
                             char *str, size_t str_len) {
    write_str_to_buf(buf, offset, len, len / 2, str, str_len);
}

void write_str_to_buf(char **buf, size_t offset, size_t len, size_t index,
                      char *str, size_t str_len) {
    memcpy(buf[index] + offset, str, str_len);
}

terminalSize get_terminal_size(void) {
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

    // if the above methods fail just choose a semi reasonable guess
    // this is more for GDB/LLDB such that they don't just fail on us since for
    // whatever reason you can't grab a col/row size.
    if (cols == 0) {
        cols = 80;
        rows = 80;
    }

    return (terminalSize){.width = cols, .height = rows};
}

void sleep_ms(size_t ms) {
    #ifdef WINDOWS_COMPATIBILITY
        Sleep(ms);
    #elif defined(POSIX_LEGACY)
        // has timeout as opposed to usleep
        struct timeval tv = { .tv_sec = ms / 1000, .tv_usec = (ms % 1000) * 1000 };
        select(0, NULL, NULL, NULL, &tv);
    #else
        struct timespec ts = { .tv_sec = ms / 1000, .tv_nsec = (ms % 1000) * 1000000 };
        nanosleep(&ts, NULL);
    #endif
}

void *malloc_with_oom(size_t size, char *obj_name) {
    void *obj = malloc(size);
    if (obj == NULL) {
        printf("Error: OOM; can't allocate %lu bytes for %s\n", size, obj_name);
        exit(1);
    }
    return obj;
}

void write_str_repeat_char(char *buf, size_t offset, char c, int count) {
    for (int i = 0; i < count; i++) buf[i + offset] = c;
}

void write_str_repeat_char_vert(char **buf, size_t offset, char c, int count, int index) {
    for (int i = 0; i < count; i++) buf[i][index + offset] = c;
}
