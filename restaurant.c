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
#include <cjson/cJSON.h>
#include "general.c"

struct restaurant {
    char* username;
    bool open;
    int customer_port;
    int supplier_port;
};

bool start_working(int sockfd, int port, char* username){ //return value started
    char buf[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);
    sprintf(buf, "%s Restaurant opened!\n", username);
    message msg = {buf, RESTAURANT_OPENED};
    int n = broadcast(port, &msg);
    if (n>0){
        return true;
    }
    return false;
}

bool break_rest(int sockfd, int port, char* username){
    char buf[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);
    sprintf(buf, "%s Restaurant closed!\n", username);
    message msg = {buf, RESTAURANT_CLOSED};
    int n = broadcast(port, &msg);
    if (n>0){
        return true;
    }
    return false;
}

void show_indgredients(char* username){
    char* buf = read_file(INDGREDIENTS_PATH); 

    cJSON* json = cJSON_Parse(buf);
    cJSON* item = cJSON_GetObjectItemCaseSensitive(json, username);
    item = item->child;
    while(item!=NULL){
        bzero(buf, BUFFER_SIZE);
        sprintf(buf, "%s %d\n", item->string, item->valueint);
        write(1, buf, BUFFER_SIZE);
        item = item->next;
    }

    free(buf);
}


void show_recipes(){
    char* buf = read_file(RECIPES_PATH); 

    cJSON* json = cJSON_Parse(buf);
    cJSON* food = json->child;
    int i = 1;
    while(food != NULL){
        bzero(buf, BUFFER_SIZE);
        sprintf(buf, "%d %s:\n", i, food->string);
        write(1, buf, BUFFER_SIZE);

        cJSON* indg = food->child;
        while(indg != NULL){
            bzero(buf, BUFFER_SIZE);
            sprintf(buf, "\t%s : %d\n", indg->string, indg->valueint);
            write(1, buf, BUFFER_SIZE);
            indg = indg->next;
        }
        food = food->next;
        i++;
    }
    free(buf);
}


int show_list(int port, int user_port, udp_message_code code, int max_sd, fd_set* fds, int server_fd){
    char buf[BUFFER_SIZE];
    bzero(buf,BUFFER_SIZE);
    message msg;
    sprintf(buf, "%d", port);
    msg.msg = buf;
    bzero(buf,BUFFER_SIZE);
    msg.code = code;
    int n = broadcast(port, &msg);
    fd_set working_set;
    while(1){//must disconnect connection after 3 seconds
        working_set = *fds;
        if(select(max_sd+1, &working_set, NULL, NULL, NULL)>0){
            for (int i = 0; i <= max_sd; i++) {
                if (FD_ISSET(i, &working_set)) {
                    
                    if (i == server_fd) {  
                        int new_socket = acceptClient(server_fd);
                        FD_SET(new_socket, fds);
                        if (new_socket > max_sd)
                            max_sd = new_socket;
                    }
                    
                    else { 
                        int bytes_received;
                        bytes_received = recv(i , buf, BUFFER_SIZE, 0);
                        
                        if (bytes_received == 0){ 
                            close(i);
                            FD_CLR(i, fds);
                        }
                        else{
                            printf("%d %s\n", i, buf);
                            memset(buf, 0, 1024);
                        }

                    }
                }
            }
        }
    }
    return max_sd;
}

void request_ingredient(){
    char buf[BUFFER_SIZE];
    char buf2[BUFFER_SIZE];
    char buf3[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);
    bzero(buf2, BUFFER_SIZE);
    bzero(buf3, BUFFER_SIZE);

    char* msg = "port of supplier : ";
    write(1, msg, strlen(msg));
    read(0, buf, BUFFER_SIZE);
    msg = "name of ingredient : ";
    write(1, msg, strlen(msg));
    read(0, buf2, BUFFER_SIZE);
    msg = "number of ingredient : ";
    write(1, msg, strlen(msg));
    read(0, buf3, BUFFER_SIZE);
    // int fd = make_socket(itoa(buf));

}

void show_requests_list(){

}

void answer_request(){

}

void show_history(char* username){
    char* buf = read_file(HISTORY_PATH); 

    cJSON* json = cJSON_Parse(buf);
    cJSON* item = cJSON_GetObjectItemCaseSensitive(json, username);
    item = item->child;
    while(item!=NULL){
        cJSON* food = item->child;
        bzero(buf, BUFFER_SIZE);
        sprintf(buf, "%s %s %s\n",item->string, food->string, accept_denied(food->valueint));
        write(1, buf, BUFFER_SIZE);
        item = item->next;
    }

    free(buf);
}

// void show_recipes(){

// }


int main(){
    int fd = make_socket(8080);

    start_working(fd, 8080, "ahmad");
    // bool rest_is_open = false;
    // if (argc < 2){
    //     char *msg = "please enter the port";
    //     write(2, msg, strlen(msg));
    //     return -1;
    // }

    // char buffer[1024] = {0};
    // int server_fd, new_socket, max_sd;
    // fd_set master_set, working_set;
       
    // server_fd = setUDPServer(atoi(argv[1]));

    // FD_ZERO(&master_set);
    // max_sd = server_fd;
    // FD_SET(server_fd, &master_set);

    // write(1, "Server is running\n", 18);

    // while (1) {
    //     working_set = master_set;
    //     select(max_sd + 1, &working_set, NULL, NULL, NULL);

    //     for (int i = 0; i <= max_sd; i++) {
    //         if (FD_ISSET(i, &working_set)) {
                
    //             if (i == server_fd) {  // new clinet
    //                 new_socket = acceptClient(server_fd);
    //                 FD_SET(new_socket, &master_set);
    //                 if (new_socket > max_sd)
    //                     max_sd = new_socket;
    //                 printf("New client connected. fd = %d\n", new_socket);
    //             }
                
    //             else { // client sending msg
    //                 int bytes_received;
    //                 bytes_received = recv(i , buffer, 1024, 0);
                    
    //                 if (bytes_received == 0) { // EOF
    //                     printf("client fd = %d closed\n", i);
    //                     close(i);
    //                     FD_CLR(i, &master_set);
    //                     continue;
    //                 }

    //                 printf("client %d: %s\n", i, buffer);
    //                 memset(buffer, 0, 1024);
    //             }
    //         }
    //     }

    // }

    return 0;
}