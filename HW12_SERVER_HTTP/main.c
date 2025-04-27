#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#define MAX_EVENTS 64
#define BUF_SIZE 4096
#define REQ_BUF_SIZE 8192
#define HEADER_BUF_SIZE 512

struct client_context {
    int fd;
    int file_fd;
    off_t offset;
    off_t file_size;
    char req_buf[REQ_BUF_SIZE];
    size_t req_len;
    char header_buf[HEADER_BUF_SIZE];
    size_t header_len;
    size_t header_sent;
};

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void send_response(int client_fd, int status, const char *message) {
    dprintf(client_fd,
            "HTTP/1.1 %d %s\r\n"
            "Content-Length: %zu\r\n"
            "Content-Type: text/plain\r\n"
            "\r\n"
            "%s",
            status, message, strlen(message), message);
}

void switch_to_write(int epfd, struct client_context *ctx) {
    struct epoll_event ev = {.events = EPOLLOUT | EPOLLERR | EPOLLHUP, .data.ptr = ctx};
    epoll_ctl(epfd, EPOLL_CTL_MOD, ctx->fd, &ev);
}

void handle_read(int epfd, struct client_context *ctx) {
    char buf[BUF_SIZE];
    while (1) {
        ssize_t received = recv(ctx->fd, buf, sizeof(buf), 0);
        if (received == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) break;
            perror("recv");
            close(ctx->fd);
            free(ctx);
            return;
        } else if (received == 0) {
            close(ctx->fd);
            free(ctx);
            return;
        }

        if (ctx->req_len + (size_t)received >= REQ_BUF_SIZE) {
            send_response(ctx->fd, 413, "Payload Too Large");
            close(ctx->fd);
            free(ctx);
            return;
        }

        memcpy(ctx->req_buf + ctx->req_len, buf, (size_t)received);
        ctx->req_len += (size_t)received;

        if (ctx->req_len >= 4 && strstr(ctx->req_buf, "\r\n\r\n")) {
            ctx->req_buf[ctx->req_len] = '\0';

            if (strncmp(ctx->req_buf, "GET ", 4) != 0) {
                send_response(ctx->fd, 400, "Bad Request");
                close(ctx->fd);
                free(ctx);
                return;
            }

            char *path_start = ctx->req_buf + 4;
            char *path_end = strchr(path_start, ' ');
            if (strcmp(path_start, "/") == 0) {
                path_start = "/index.html";
            }
            if (!path_end) {
                send_response(ctx->fd, 400, "Bad Request");
                close(ctx->fd);
                free(ctx);
                return;
            }
            *path_end = '\0';

            char filepath[1024];
            snprintf(filepath, sizeof(filepath), ".%s", path_start);
            if (strstr(filepath, "..")) {
                send_response(ctx->fd, 403, "Forbidden");
                close(ctx->fd);
                free(ctx);
                return;
            }

            ctx->file_fd = open(filepath, O_RDONLY);
            if (ctx->file_fd == -1) {
                if (errno == EACCES)
                    send_response(ctx->fd, 403, "Forbidden");
                else
                    send_response(ctx->fd, 404, "Not Found");
                close(ctx->fd);
                free(ctx);
                return;
            }

            struct stat st;
            if (fstat(ctx->file_fd, &st) == -1 || !S_ISREG(st.st_mode)) {
                send_response(ctx->fd, 404, "Not Found");
                close(ctx->file_fd);
                close(ctx->fd);
                free(ctx);
                return;
            }

            ctx->file_size = st.st_size;
            ctx->offset = 0;

            ctx->header_len = snprintf(ctx->header_buf, HEADER_BUF_SIZE,
                "HTTP/1.1 200 OK\r\n"
                "Content-Length: %ld\r\n"
                "Content-Type: application/octet-stream\r\n"
                "\r\n", st.st_size);
            ctx->header_sent = 0;

            switch_to_write(epfd, ctx);
            break;
        }
    }
}

void handle_write(int epfd, struct client_context *ctx) {
    while (ctx->header_sent < ctx->header_len) {
        ssize_t sent = send(ctx->fd, ctx->header_buf + ctx->header_sent, ctx->header_len - ctx->header_sent, 0);
        if (sent == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            perror("send header");
            close(ctx->fd);
            free(ctx);
            return;
        }
        ctx->header_sent += (size_t)sent;
    }

    while (ctx->offset < ctx->file_size) {
        ssize_t sent = sendfile(ctx->fd, ctx->file_fd, &ctx->offset, ctx->file_size - ctx->offset);
        if (sent <= 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) return;
            perror("sendfile");
            close(ctx->file_fd);
            close(ctx->fd);
            free(ctx);
            return;
        }
    }

    close(ctx->file_fd);
    epoll_ctl(epfd, EPOLL_CTL_DEL, ctx->fd, NULL);
    close(ctx->fd);
    free(ctx);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <directory> <ip:port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (chdir(argv[1]) != 0) {
        perror("chdir");
        return EXIT_FAILURE;
    }

    char *colon = strchr(argv[2], ':');
    if (!colon) {
        fprintf(stderr, "Invalid address:port format\n");
        return EXIT_FAILURE;
    }
    *colon = '\0';
    const char *ip_str = argv[2];
    int port = atoi(colon + 1);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    set_nonblocking(server_fd);

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons((uint16_t)port)
    };
    if (inet_pton(AF_INET, ip_str, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        return EXIT_FAILURE;
    }

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    int epfd = epoll_create1(0);
    if (epfd < 0) {
        perror("epoll_create1");
        return EXIT_FAILURE;
    }

    struct epoll_event ev = {.events = EPOLLIN, .data.fd = server_fd};
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_fd, &ev);

    struct epoll_event events[MAX_EVENTS];
    while (1) {
        int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; i++) {
            if (events[i].data.fd == server_fd) {
                int client_fd = accept(server_fd, NULL, NULL);
                if (client_fd >= 0) {
                    set_nonblocking(client_fd);
                    struct client_context *ctx = calloc(1, sizeof(struct client_context));
                    ctx->fd = client_fd;
                    struct epoll_event cev = {.events = EPOLLIN | EPOLLERR | EPOLLHUP, .data.ptr = ctx};
                    epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &cev);
                }
            } else {
                struct client_context *ctx = events[i].data.ptr;
                if (events[i].events & EPOLLIN) {
                    handle_read(epfd, ctx);
                } else if (events[i].events & EPOLLOUT) {
                    handle_write(epfd, ctx);
                } else {
                    close(ctx->fd);
                    if (ctx->file_fd != -1) close(ctx->file_fd);
                    free(ctx);
                }
            }
        }
    }

    close(server_fd);
    return EXIT_SUCCESS;
}
