#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"

int default_oracle(char* dir_name,int trial,int return_code){
    return return_code;
}

void init_config(config_t * config){
    config->inp_arg.f_min_len = 10;
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_char_start = 32;
    config->inp_arg.f_char_range = 32;

    config->oracle = default_oracle;          //set the default oracle
    config->trial = 10;

    config->run_arg.timeout = 2;
    config->run_arg.cmd_args = NULL;
    
    memset(config->run_arg.seed_dir,0,sizeof(char)*PATH_MAX);
    memset(config->run_arg.src_dir,0,sizeof(char)*PATH_MAX);
    
    for(int i = 0; i < NUM_OF_MAX ; i++){
        config->run_arg.src_file[i] = NULL;
        config->run_arg.seed_file_name[i] = NULL;
    }
    
    config->run_arg.seed_file_num = 0;
    config->run_arg.args_num = 0;
    config->run_arg.src_file_num = 0;
    config->run_arg.binary_path = NULL;
    config->run_arg.fuzz_type = 0;
    config->coverage = 0;
}