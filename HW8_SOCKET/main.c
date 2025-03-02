#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 256

void print_config(const char* target_file, const char* socket_path, const int *flag) {
    printf("Конфигурация:\n");
    printf("Path file: %s\n", target_file);
    printf("Path socket: %s\n", socket_path);
    printf("Demon mode: %d\n", *flag);
}

int init_socket(const char *path_sock) {
    struct sockaddr_un addr;
    int connect_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (connect_socket == -1) {
        perror("Error open socket");
        exit(EXIT_FAILURE);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path_sock, sizeof(addr.sun_path)-1);
    int ret = bind(connect_socket, (struct sockaddr*)&addr, sizeof(addr));
    if (ret == -1) {
        perror("Error bind socket");
        exit(EXIT_FAILURE);
    }

    ret = listen(connect_socket, 5);
    if (ret == -1) {
        perror("Error listen socket");
        exit(EXIT_FAILURE);
    }
    return connect_socket;
}

void demotization() {
    int daemon_status = daemon(0,0);
    if (daemon_status < 0) {
        perror("Daemon error");
        exit(EXIT_FAILURE);
    }
}

off_t size_file(const char *path_file) {
    struct stat file_data;
    if (stat(path_file, &file_data)){
        printf("Файл не существует");
        return -1;
    }
    return file_data.st_size;
}


int main()
{
    config_t cfg;
    char buffer[BUFFER_SIZE];
    const int flag_daemon;
    const char *path_file, *socket_path;
    config_init(&cfg);
    if (!config_read_file(&cfg, "test.conf")) {
        perror("Error config");
        exit(EXIT_FAILURE);
    }
    config_lookup_int(&cfg, "daemon_mode", (int *)&flag_daemon);
    if (!(config_lookup_string(&cfg, "target_file", &path_file) &&
        config_lookup_string(&cfg, "socket_path", &socket_path))){
            perror("Check test.conf");
            exit(EXIT_FAILURE);
        }
    print_config(path_file, socket_path, &flag_daemon);
    if (flag_daemon) {
        demotization();
    }
    int socket = init_socket(socket_path);
    for (;;) {
        int new_socket = accept(socket, NULL, NULL);
        if (new_socket == -1) {
            perror("Error accept socket");
            exit(EXIT_FAILURE);
        }
        off_t size_data = size_file(path_file);
        if (size_data == -1) {
            send(new_socket,"File not found", sizeof("File not found"),0);
        }
        else {
            memset(buffer, 0, BUFFER_SIZE);
            snprintf(buffer, BUFFER_SIZE, "Размер файла: %ld байт\n", size_data);  
            send(new_socket, buffer, sizeof(buffer), 0);
        }
        close(new_socket);
    }
    close(socket);
    unlink(socket_path);
    exit(EXIT_SUCCESS);
}