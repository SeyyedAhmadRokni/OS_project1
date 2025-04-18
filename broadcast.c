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

const int BUFF_SIZE = 1024;

int setupBroadcast(int port) {
    
    char* ip = "127.0.0.1";

    struct sockaddr_in address;
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_fd < 0){
        char *err_msg = "socket not created";
        write(2, err_msg, strlen(err_msg));
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    
    memset(&address, '\0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ip);

    int n = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    if (n<0){
        printf("not binded");
        return server_fd;
    }

    return server_fd;
}

int main(int argc, char **argv){

    if (argc < 2){
        char *msg = "please enter a port";
        write(2, msg, strlen(msg));
        exit(1);
    }

    int port = atoi(argv[1]);
    int sockfd = setupBroadcast(port);
    // printf("%d\n",sockfd);

    int current_max = sockfd;
    fd_set ready_sockets, server_sockets;
    FD_ZERO(&server_sockets);
    FD_SET(STDIN_FILENO, &server_sockets);
    FD_SET(sockfd, &server_sockets);

    struct sockaddr_in client;
    char buffer[BUFF_SIZE];
    
    while(1){
        ready_sockets = server_sockets;
        if(select(current_max+1, &ready_sockets, NULL, NULL, NULL) > 0){
            for (int i = 0; i <= current_max; i++){
                if (FD_ISSET(i, &ready_sockets)){
                    // printf("seted fd: %d \n", i);
                    // printf("strlen buff : %d", (int)strlen(buffer));
                    bzero(buffer, 0);
                    int client_len = sizeof(client);
                    if (i==STDIN_FILENO){
                        int received = recvfrom(STDIN_FILENO, buffer, BUFF_SIZE, 0, (struct sockaddr*)&client, &client_len);
                        printf("len buffer: %d\n", (int)strlen(buffer));
                        int send_key = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&client, sizeof(client_len));
                        printf("sends : %d\n",send_key);
                    }
                    else if (i == sockfd){
                        int received = recvfrom(i, buffer, BUFF_SIZE, 0, (struct sockaddr*)&client, &client_len);
                        if (received > 0){
                            write(1, buffer, strlen(buffer));
                        }
                    }
                }
            }
        }
    }
}