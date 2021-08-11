#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "runner.h"

void Runner_init(Runner* runner){
    runner->input = 0x0;
    runner->outcome = 0x0;
    runner->inp_size = 0;

    runner->free_inp = free_input;
    runner->run = run;
}

int run(Runner* self,char* input,int inp_size){
    if(inp_size > 0){
    self->input = (char*)malloc(sizeof(char)*inp_size);
    
    if(self->input == NULL){
        perror("malloc failed\n");
        return -1;
    }

    char* mem_checker = memcpy(self->input,input,inp_size);
    if(mem_checker == NULL){
        perror("Runner Class memcpy failed in run function\n");
        return -1;
    }
    }else if(inp_size ==0){
        self->input = 0x0;
    }else{
        perror("Wrong input size\n");
        return -1;
    }

    self->inp_size = inp_size;
    self->outcome = UNRESOLVED;

    return 0;
}

void free_input(Runner runner){
    if(runner.inp_size != 0){
        free(runner.input);
    }else{
        printf("input size is Zero\n");
    }
}