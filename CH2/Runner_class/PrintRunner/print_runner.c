#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "print_runner.h"

void PrintRuner_init(Runner * print_runner){
    print_runner->input = 0x0;
    print_runner->outcome = 0x0;
    print_runner->free_inp = free_input;
    print_runner->run = PrintRunner_run;
}

int PrintRunner_run(Runner* print_runner,char* input, int inp_size){
    int ret = run(print_runner,input,inp_size);
    for(int i = 0; i < print_runner->inp_size; i++){
        printf("%d %c\n",i,print_runner->input[i]);
    }
    printf("\n");

    return ret;    
}

int main(){
    Runner print_runner;
    PrintRuner_init(&print_runner);
    char input[] = "\0he\0llo";

    print_runner.run(&print_runner,input,sizeof(input));
    print_runner.free_inp(print_runner);
}