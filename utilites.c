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
// char [][]split(char* string){

// }

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

char** split(char* inp, char* sep){
    char** tokens = (char **)calloc(5, sizeof(char*));
    int size = 5;
    char* token = strtok(inp, sep);
    int i=0;
    while(token != NULL){
        tokens[i] = token;
        token = strtok(NULL, sep);
        i++;
        if (i==size-1){
            size+=5;
            tokens = (char **)realloc(tokens, size * sizeof(char*));
        }
    }
    tokens[i] = NULL;
    return tokens;
}


int main(){
    char buffer[BUFFER_SIZE];
    read(0, buffer, BUFFER_SIZE);
    char** tokens = split(buffer, " ");
    for (int i = 0; i < MAX_CHAR_SIZE && tokens[i] != NULL; i++){
        printf("%s ", tokens[i]);
    }
    
}