/* author: pjankows@gmail.com */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

struct addrinfo* prepare_addrinfo(char *host, char *port)
{
    int status;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(host, port, &hints, &res);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        res = NULL;
    }
    return res;
}

int get_socket(struct addrinfo *res)
{
    int s = -1;
    if (res != NULL) {
        s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    }
    return s;
}

int get_port(struct addrinfo *res)
{
    unsigned short int port;
    if (res->ai_family == AF_INET) {
        port = ((struct sockaddr_in*)res->ai_addr)->sin_port;
    }
    else {
        port = ((struct sockaddr_in6*)res->ai_addr)->sin6_port;
    }
    return ntohs(port);
}

int bind_socket(struct addrinfo *res)
{
    int s, b;
    s = get_socket(res);
    if (s != -1) {
        b = bind(s, res->ai_addr, res->ai_addrlen);
        if (b == -1) {
            fprintf(stderr, "bind to port %d failed: ", get_port(res));
            perror(NULL);
            close(s);
            s = -1;
        }
    }
    return s;
}

int main(int argc, char *argv[])
{
    int s;
    struct addrinfo *res;
    res = prepare_addrinfo(argv[1], argv[2]);
    printf("%p\n", res);
    s = bind_socket(res);
    printf("%d\n", s);
    freeaddrinfo(res);
    return 0;
}