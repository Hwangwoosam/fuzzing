#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/create_inp.h"
// create random string input
void create_inp(char* random_inp,int * inp_size,config_t test_config){
    
    int string_length = rand()%(test_config.inp_arg.f_max_len+1);
    printf("%d size\n",test_config.inp_arg.f_max_len);
    *inp_size = string_length; 
    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(test_config.inp_arg.f_char_range + 1) + test_config.inp_arg.f_char_start;
        random_inp[i] = tmp;
    }
    random_inp[string_length] = '\0';

    return;
}