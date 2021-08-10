#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "../include/0_fuzzer.h"
#include "../include/5_program_runner.h"
#include "../include/6_fuzzer_class.h"

fuzzer_t* init_class(int max_length,int min_length,int char_start,int char_range){
    fuzzer_t* temp = (fuzzer_t*)malloc(sizeof(fuzzer_t));
    temp->min_length = min_length;
    temp->max_length = max_length;
    temp->char_start = char_start;
    temp->char_range = char_range;
    return temp;
}

char* fuzz(fuzzer_t* input){
    int string_length = rand()%(input->max_length-input->min_length+1)+input->min_length;
    char * out = (char*)malloc(sizeof(char)*string_length+1);
    for(int i = 0; i < string_length; i++){
        out[i] = rand()%(input->char_range+1) + input->char_start;
    }
    out[string_length] = '\0';
    return out;
}

void run_class(char* program, fuzzer_t* input,int num){
    for(int i = 0; i < num; i++){
        char* str = fuzz(input);
        result_t* tmp = subprocess(program,str);
        assert(strcmp(str,tmp->output)== 0);
        assert(strcmp(PASS,tmp->outcome) == 0);
        printf("CompletedProcess:\nargs= \'%s\'\nreturncode = %d\nstdout= \'%s\'\nstderr= \'%s\'\nprogram_test= \'%s\'\n\n",program,tmp->return_code,tmp->output,tmp->error,tmp->outcome);
        free(tmp);
        free(str);
    } 
}
