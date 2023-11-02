#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <stdbool.h>
#include "utilites.c"

typedef enum udp_message_code{
    ADD_TO_SYSTEM = 1,
    USERNAME_UNIQENESS,
    USERNAME_UNIQENESS_ANS,
    RESTAURANT_OPENED,
    RESTAURANT_CLOSED,
    SUPPLIERS_LIST,
    RESTAURANT_LIST,
    SUPPLIERS_LIST_ANS,
    RESTAURANT_LIST_ANS,
}udp_message_code;

typedef struct message {
    char* msg;
    udp_message_code code;
}message;

struct sockaddr_in make_sock_addr(int port){
    char *ip = "127.0.0.1";

    struct sockaddr_in addr;

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    return addr;
}

int make_socket(int port){

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (sockfd < 0){
        return -1;
    }
    return sockfd;
}

int setUDPserver(int port){

    char *ip = "127.0.0.1";

    struct sockaddr_in server_addr = make_sock_addr(port);
    int sockfd = make_socket(port);

    if (sockfd<0){
        return sockfd;
    }

    int n = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n<0){
        return -2;
    }

    return sockfd;

}

int broadcast(int port, message* msg){
    char buf[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);
    strcpy(&buf[1], msg->msg);
    buf[0] = msg->code;
    int fd = make_socket(port);
    struct sockaddr_in adr = make_sock_addr(port);
    int n = sendto(fd, buf, strlen(buf), 0,(struct sockaddr*)&adr, sizeof(adr) );
    return n;
}

message* receive_broadcast(int port){
    char* buf = calloc(BUFFER_SIZE, sizeof(char));
    bzero(buf, BUFFER_SIZE);
    int fd = make_socket(port);
    struct sockaddr_in adr = make_sock_addr(port);
    int adr_size = sizeof(adr);
    int n = recvfrom(fd, buf, BUFFER_SIZE, 0, (struct sockaddr*)&adr, &adr_size);

    message* msg = malloc(sizeof(message));
    msg->code = buf[0];
    msg->msg = &buf[1];

    return msg;
}


int setupServer(int port) {
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    
    listen(server_fd, 4);

    return server_fd;
}

int acceptClient(int server_fd) {
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);

    return client_fd;
}