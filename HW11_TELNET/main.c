#include <stdio.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MSG_LEN 1024
#define BUF_SIZE 4096
#define HOST "telehack.com"
#define SERVICE "telnet"

int connectHost() {
    struct addrinfo hints, *res, *validAddr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int status, sockfd;
    if ((status = getaddrinfo(HOST, SERVICE, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }
    for (validAddr = res; validAddr != NULL; validAddr = validAddr->ai_next) {
        sockfd = socket(validAddr->ai_family, validAddr->ai_socktype, validAddr->ai_protocol);
        if (sockfd == -1) {
            continue;
        }
        if (connect(sockfd, validAddr->ai_addr, validAddr->ai_addrlen) != -1) {
            break;
        }
        }
    if (validAddr == NULL) {
        fprintf(stderr, "failed to connect\n");
        exit(EXIT_FAILURE);
        }
    freeaddrinfo(res);
    return sockfd;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("no arguments");
        exit(EXIT_FAILURE);
    }
    int sockfd;
    char msg[MSG_LEN];
    sprintf(msg, "figlet /%s %s", argv[1], argv[2]);
    sockfd = connectHost();
    char buffer[BUF_SIZE];
    int len = 0, r = 0;
    while((r = recv(sockfd, &buffer[len], BUF_SIZE, 0)) > 0) {
        len += r;
        if (buffer[len-1] == '.' && buffer[len-2] == '\n') break;
    }
    if ((send(sockfd, msg, strlen(msg), 0)) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }
    len = 0, r = 0;
    if ((recv(sockfd, &buffer[len], BUF_SIZE, 0)) == -1) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    printf("%s", buffer);
    exit(EXIT_SUCCESS);
}