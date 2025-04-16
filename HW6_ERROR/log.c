#include "log.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <execinfo.h>
#include <pthread.h>

static FILE *log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

static const char *level_strings[] = {
    "DEBUG", "INFO", "WARNING", "ERROR"
};

bool log_init(const char *filename) {
    pthread_mutex_lock(&log_mutex);
    if (log_file) {
        pthread_mutex_unlock(&log_mutex);
        return false; // already initialized
    }
    log_file = fopen(filename, "a");
    pthread_mutex_unlock(&log_mutex);
    return log_file != NULL;
}

void log_message(log_level_t level, const char *file, int line, const char *fmt, ...) {
    if (!log_file) return;

    pthread_mutex_lock(&log_mutex);
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    char timestr[20];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", lt);

    fprintf(log_file, "[%s] [%s] %s:%d: ", timestr, level_strings[level], file, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(log_file, fmt, args);
    va_end(args);
    fprintf(log_file, "\n");

    if (level == LOG_ERROR) {
        void *buffer[100];
        int nptrs = backtrace(buffer, 100);
        backtrace_symbols_fd(buffer, nptrs, fileno(log_file));
    }

    fflush(log_file);
    pthread_mutex_unlock(&log_mutex);
}

void log_close() {
    pthread_mutex_lock(&log_mutex);
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
    pthread_mutex_unlock(&log_mutex);
}