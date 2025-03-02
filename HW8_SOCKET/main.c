#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

int main()
{
    config_t cfg;
    const int *flag_daemon;
    config_init(&cfg);
    if (!config_read_file(&cfg, "test.conf")) {
        perror("Error config");
        return(EXIT_FAILURE);
    }
    config_lookup_bool(&cfg, "daemon_mode", &flag_daemon);
    printf("name = %d\n", flag_daemon);
    return 0;
}