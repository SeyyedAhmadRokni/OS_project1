#ifndef _GEN_
#define _GEN_
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
#include <signal.h>
#include "serv_func.c"

char* INDGREDIENTS_PATH = "indgredients.json";
char* RECIPES_PATH = "recipes.json";
char* HISTORY_PATH = "history.json";

typedef enum job{
    RESTAURANT,
    SUPPLIER,
    CUSTOMER
}job;

typedef enum command {
    START,
    CLOSE,
    INDG_LIST,
    RECIPES_LIST,
    SUPP_LIST,
    REQ_INDG,
    SHOW_REQ,
    ANS_REQ,
    SHOW_HIST,
    SHOW_RESTAURANTS,
    SHOW_MENU,
    ORDER_FOOD,
    NO_ONE,
}command;

command command_detector(char* msg){
    if(strcmp(msg, "start working") == 0){
        return START;
    }
    else if(strcmp(msg, "break") == 0){
        return CLOSE;
    }
    else if(strcmp(msg, "show ingredients") == 0){
        return INDG_LIST;
    }
    else if(strcmp(msg, "show recipes") == 0){
        return RECIPES_LIST;
    }
    else if(strcmp(msg, "show suppliers") == 0){
        return SUPPLIERS_LIST;
    }
    else if(strcmp(msg, "request ingredient") == 0){
        return REQ_INDG;
    }
    else if(strcmp(msg, "show requests list") == 0){
        return SHOW_REQ;
    }
    else if(strcmp(msg, "answer request") == 0){
        return ANS_REQ;
    }
    else if(strcmp(msg, "show sales history") == 0){
        return SHOW_HIST;
    }
    else if(strcmp(msg, "show restaurants") == 0){
        return SHOW_RESTAURANTS;
    }
    else if(strcmp(msg, "show menu") == 0){
        return SHOW_MENU;
    }
    else if(strcmp(msg, "order food") == 0){
        return ORDER_FOOD;
    }
    else{
        return NO_ONE;
    }
}

void alarm_hand(int signum){
    return ;
}

char* signup(int port){

    char *msg = "Please enter your username : ";
    write(1, msg, strlen(msg));
    char* buf = calloc(BUFFER_SIZE, sizeof(char));
    
    int fd = make_socket(port);
    fd_set fds, temp;
    struct sockaddr_in adr = make_sock_addr(port);
    message sent_msg;
    signal(SIGALRM, alarm_hand);

    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    FD_SET(STDIN_FILENO, &fds);
    int a;

    while(1){
        temp = fds;
        a = select(fd+1, &temp, NULL, NULL, NULL);
        if (a < 0){
            return sent_msg.msg;
        }
        else if (a>0){
            for (int i = 0; i <= fd; i++){
                if (FD_ISSET(i, &temp)){
                    if (i==fd){
                        message* msg = receive_broadcast(port);
                        printf("%s", msg->msg);
                        if (msg->code == USERNAME_UNIQENESS_ANS ){
                            int n = strcmp(sent_msg.msg, msg->msg);
                            if (n!=0){
                                return sent_msg.msg;
                            }
                            else{
                                char *msg = "Please enter a valid username : ";
                                write(1, msg, strlen(msg));
                            }
                        }
                    }
                    if (i==STDIN_FILENO){
                        bzero(buf, BUFFER_SIZE);
                        read(0, buf, BUFFER_SIZE);
                        sent_msg.code = USERNAME_UNIQENESS;
                        sent_msg.msg = buf;
                        int x = broadcast(port, &sent_msg);
                        alarm(3);
                    }
                }
            }
        }
    }
    return NULL;
}

int main(){
    signup(8080);
    return 0;
}
#endif