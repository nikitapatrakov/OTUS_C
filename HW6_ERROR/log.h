#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

#define LOG_DEBUG_MSG(fmt, ...) log_message(LOG_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO_MSG(fmt, ...) log_message(LOG_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARNING_MSG(fmt, ...) log_message(LOG_WARNING, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR_MSG(fmt, ...) log_message(LOG_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

typedef enum {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} log_level_t;

bool log_init(const char *filename);
void log_message(log_level_t level, const char *file, int line, const char *fmt, ...);
void log_close();

#endif // LOG_H