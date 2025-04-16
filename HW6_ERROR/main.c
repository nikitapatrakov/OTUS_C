#include "log.h"
#include <stdlib.h>

void test_function() {
    LOG_DEBUG_MSG("Debugging %s", "test_function");
    LOG_INFO_MSG("Some information");
    LOG_WARNING_MSG("A warning occurred");
    LOG_ERROR_MSG("An error occurred");
}

int main() {
    if (!log_init("log.txt")) {
        fprintf(stderr, "Failed to initialize log\n");
        return EXIT_FAILURE;
    }

    test_function();

    log_close();
    return EXIT_SUCCESS;
}
