#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <stdarg.h>


#define COLORIZE(text, color_code) "\033[" #color_code "m" text "\033[0m"

enum Log_Level {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

void log(Log_Level level, const char *fmt, ...) {
    switch (level) {
        case LOG_INFO: {
            fprintf(stderr, "[" COLORIZE("INFO", 32) "] ");
        } break;

        case LOG_WARNING: {
            fprintf(stderr, "[" COLORIZE("WARNING", 33) "] ");
        } break;

        case LOG_ERROR: {
            fprintf(stderr, "[" COLORIZE("ERROR", 31) "] ");
        } break;

        default: {
            *(volatile int *)0 = 0;
        } break;
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}


#endif // LOG_H_
