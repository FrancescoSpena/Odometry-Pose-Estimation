#include <stdio.h>
#include <stdlib.h>
#include "beth_client.h"

BethClient client;

//argv[0] is name
int main(int argc, char** argv){
    if(argc <= 1){
        printf("usage: %s <device> <speed>", argv[0]);
        return 0;
    }
    int speed = atoi(argv[2]);
    char* device = argv[1];
    BethClient_init(&client,device,speed);
    return 0;
}