#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/config.h"

int default_oracle(char* dir_name,int trial,int return_code){
    return return_code;
}

void init_config(config_t * config,run_arg_t* run_arg,input_arg_t* inp_arg){
    inp_arg->f_min_len = 10;
    inp_arg->f_max_len = 100;
    inp_arg->f_char_start = 32;
    inp_arg->f_char_range = 32;

    config->oracle = default_oracle;          //set the default oracle
    config->trial = 10;

    run_arg->timeout = 2;
    run_arg->cmd_args = NULL;
    
    memset(run_arg->seed_dir,0,sizeof(char)*PATH_MAX);
    memset(run_arg->src_dir,0,sizeof(char)*PATH_MAX);
    
    for(int i = 0; i < NUM_OF_MAX ; i++){
        run_arg->src_file[i] = NULL;
        run_arg->seed_inp[i] = NULL;
    }
    
    run_arg->seed_file_num = 0;
    run_arg->args_num = 0;
    run_arg->src_file_num = 0;
    run_arg->binary_path = NULL;
    run_arg->fuzz_type = 0;
    config->coverage = 0;
}