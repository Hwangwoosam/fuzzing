#include "create_inp.h"
#include <stdlib.h>

void fuzzer(char* random_inp,int * inp_size,input_arg_t inp_config){
    
    int string_length = rand()%(inp_config.f_max_len+1);
    *inp_size = string_length; 
    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(inp_config.f_char_range + 1) + inp_config.f_char_start;
        random_inp[i] = tmp;
    }
    random_inp[string_length] = '\0';

    return;
}