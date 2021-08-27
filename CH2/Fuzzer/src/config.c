#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"

int default_oracle(char* dir_name,int trial,int return_code){
    return return_code;
}

//set the default value of config
void init_config(test_config_t * config){
    
    //set the input argument
    config->inp_arg.f_min_len = 10;
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_char_start = 32;
    config->inp_arg.f_char_range = 32;

    config->oracle = default_oracle;          //set the default oracle
    config->trial = 10;

    //set the input argument
    config->run_arg.timeout = 2;
    config->run_arg.cmd_args = NULL;
    config->run_arg.args_num = 0;
    config->run_arg.src_path = NULL;
    config->run_arg.binary_path = NULL;
    config->run_arg.fuzz_type = 0;
}