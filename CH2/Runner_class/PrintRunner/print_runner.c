#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "print_runner.h"

void PrintRuner_init(Runner * print_runner){
    print_runner->input = 0x0;
    print_runner->outcome = 0x0;
    print_runner->inp_size = 0;
    print_runner->free_inp = free_input;
    print_runner->run = PrintRunner_run;
}

int PrintRunner_run(Runner* self,char* input, int inp_size){

    if(inp_size > 0){
        self->input = (char*)malloc(sizeof(char)*inp_size);

        if(self->input == NULL){
            perror("malloc failed\n");
            return -1;
        }

        self->input = memcpy(self->input,input,inp_size);
        if(self->input == NULL){
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

    write(1,self->input,self->inp_size);
    
    // for(int i = 0; i < self->inp_size; i++){
    //      putchar(self->input[i]);
    // }
    // printf("\n"); 

    return 0;
}

int main(){
    Runner print_runner;
    PrintRuner_init(&print_runner);
    char input[] = "\0he\0llo";

    print_runner.run(&print_runner,input,sizeof(input));
    print_runner.free_inp(print_runner);
}