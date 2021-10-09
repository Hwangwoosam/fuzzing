#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/create_inp.h"

int main(){
    srand(time(NULL));
    char str[] = "helloworld";
    int length = strlen(str);
    char* target = (char*)malloc(sizeof(char)*(length + 5));
    int byte_size[3] = {1,2,4};
    int byte = 0;
    for(int i = 0; i < length ; i++){
        if(byte_size[2-i] <= length){
           byte = rand()%(3-i);
           break;
        }
    }
    known_integers_insert(str,target,length,byte_size[byte]);
    // printf("%s %s\n",str,target);
}