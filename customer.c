#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <cjson/cJSON.h>

const int BUFFER_SIZE = 1024;

char* RECIPES_PATH = "recipes.json";

void show_restaurants(){

}

void show_menu(){
    int fd = open(RECIPES_PATH, O_RDONLY);
    if(fd < 0){
        return;
    }
    char buf[BUFFER_SIZE]; 
    bzero(buf, BUFFER_SIZE);
    read(fd, buf, BUFFER_SIZE);
    close(fd);

    cJSON* json = cJSON_Parse(buf);
    cJSON* food = json->child;
    int i = 1;
    while(food != NULL){
        bzero(buf, BUFFER_SIZE);
        sprintf(buf, "%d %s:\n", i, food->string);
        write(1, buf, BUFFER_SIZE);
        food = food->next;
        i++;
    }
}

void order_food(){


}