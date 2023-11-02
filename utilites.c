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

#endif