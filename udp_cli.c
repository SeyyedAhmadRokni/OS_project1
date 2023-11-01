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

const int SIZE = 1024;
int main(int argc, char **argv){
    
    char *ip = "127.0.0.1";
    int port = 8080;

    int sockfd;
    struct sockaddr_in addr;
    char buffer[1024];
    socklen_t addr_size;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("socket: %d\n", sockfd);

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    bzero(buffer, 1024);
    strcpy(buffer, "Hello man\n");

    int x = sendto(sockfd, buffer, 1024, 0, (struct sockaddr*)& addr, sizeof(addr));

    bzero(buffer, 1024);
    sprintf(buffer, "code of send : %d \n", x);
    write(1, buffer, strlen(buffer));
    addr_size = sizeof(addr);
    recvfrom(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&addr, &addr_size);
    write(1, buffer, strlen(buffer));


}