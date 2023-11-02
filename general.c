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
#include "udp_func.c"


const int BUFFER_SIZE = 1024;
const int MAX_CHAR_SIZE = 100;
char* INDGREDIENTS_PATH = "indgredients.json";
char* RECIPES_PATH = "recipes.json";
char* HISTORY_PATH = "history.json";

bool signup(char* username, int port){

    char *msg = "Please enter your username : ";
    write(1, msg, strlen(msg));
    char buf[BUFFER_SIZE];
    
    int fd = make_socket(port);
    fd_set fds, temp;
    struct sockaddr_in adr = make_sock_addr(port);

    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    FD_SET(STDERR_FILENO, &fds);
    struct timeval tv;
    tv.tv_sec = 3;
    tv.tv_usec = 0;

    while(1){
        temp = fds;
        if (select(fd+1, &fds, NULL, NULL, &tv)>0){
            for (int i = 0; i <= fd; i++){
                if (FD_ISSET(i, &temp)){
                    if (i==fd){
                        char* buf2 = receive_broadcast(port);
                        int n = strcmp(username, buf2);
                        if (n==0){
                            return true;
                        }
                        else{
                            char *msg = "Please enter a valid username : ";
                            write(1, msg, strlen(msg));
                        }
                    }
                    if (i==0){
                        bzero(buf, BUFFER_SIZE);
                        read(0, buf, BUFFER_SIZE);
                        broadcast(port, buf);
                    }
                }
            }
        }
    }
    return false;
}