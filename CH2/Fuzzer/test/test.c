#include "../include/create_inp.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(){
    srand(time(NULL));
    char inp_str[] ="helloworld";
    for(int i = 0; i < 10; i++){
    char* inp_result = mutate(inp_str);
    printf("%s\n",inp_result);
    free(inp_result);
    }
}