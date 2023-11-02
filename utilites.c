#ifndef _UTIL_
#define _UTLI_
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

const int BUFFER_SIZE = 1024;
const int MAX_CHAR_SIZE = 100;

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

char* read_file(char* filepath){
    int fd = open(filepath, O_RDONLY);
    if(fd < 0){
        fd = creat(filepath, O_RDONLY);
        close(fd);
        return NULL;
    }
    char* buf = calloc(BUFFER_SIZE, sizeof(char)); 
    read(fd, buf, BUFFER_SIZE);
    close(fd);
    return buf;
}

int write_file(char* filepath, char* msg){
    int fd = open(filepath, O_WRONLY | O_CREAT);
    if(fd < 0){
        fd = creat(filepath, O_RDONLY);
        close(fd);
        return 0;
    }
    write(fd, msg, strlen(msg));
    close(fd);
    return fd;
}

char* yes_to_accept(char* a){

    if(strcmp(a, "yes")){
        return "accepted";
    }
    return "denied";
}

#endif