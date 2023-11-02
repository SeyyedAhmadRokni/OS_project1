#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <strings.h>

int main(int argc, char **argv){
    
    char *ip = "127.0.0.1";
    int port = 8080;

    int sockfd;
    int n;
    struct sockaddr_in server_addr;
    char buffer[1024];
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("socket: %d\n", sockfd);

    if (sockfd < 0){
        char *err_msg = "socket not created";
        write(2, err_msg, strlen(err_msg));
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    bzero(buffer, 1024);

    n = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n<0){
        char *err_msg = "bind errored";
        write(2, err_msg, strlen(err_msg));
    }


    struct sockaddr_in client_addr;
    memset(buffer, '\0', 1024);
    addr_size = sizeof(client_addr);
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_size);
    write(1, buffer, strlen(buffer));
    bzero(buffer, 1024);
    sprintf(buffer, "Wellcome to UDP");
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));

}