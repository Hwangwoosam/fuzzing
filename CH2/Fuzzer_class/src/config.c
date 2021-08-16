#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"

int default_oracle(char* dir_name,int trial,int return_code){
    return return_code;
}

void init_config(test_config_t * config){
    config->inp_arg.f_min_len = 10;
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_char_start = 32;
    config->inp_arg.f_char_range = 32;

    config->oracle = default_oracle;          //set the default oracle
    config->trial = 10;

    config->run_arg.timeout = 2;
    config->run_arg.cmd_args = NULL;
    config->run_arg.args_num = 0;

    char* mem_check;
    mem_check = memset(config->run_arg.binary_path,0,PATH_MAX);
    if(mem_check == NULL){
        perror("binary path initialize failed\n");
        exit(0);
    }
}