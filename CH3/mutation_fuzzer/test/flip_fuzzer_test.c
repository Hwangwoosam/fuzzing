#include <stdio.h>
#include <stdlib.h>
#include "../include/fuzzer.h"
#include "../include/coverage.h"
#include "../include/config.h"

void set_configs(config_t* config){
    config->run_arg.binary_path = "./gcovtest/a.out";
    strcpy(config->run_arg.src_dir,"/mnt/c/projects/git/fuzzing/CH3/mutation_fuzzer/test/gcovtest");

    char src_file_0[] = "flip_random.gcno";
    char src_file_1[] = "create_inp.gcno";

    config->run_arg.src_file[0] = (char*)malloc(sizeof(char)*(strlen(src_file_0) + 1));
    strcpy(config->run_arg.src_file[0],src_file_0);

    config->run_arg.src_file[1] = (char*)malloc(sizeof(char)*(strlen(src_file_1) + 1));
    strcpy(config->run_arg.src_file[1],src_file_1);
    
    config->run_arg.src_file_num = 2;

    strcpy(config->run_arg.seed_dir,"./seed_dir");
    config->run_arg.seed_file_num = 0;

    config->inp_arg.f_char_start = 32;
    config->inp_arg.f_char_range = 127;

    config->trial = 100;
    
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_min_len = 100;
    
    config->run_arg.cmd_args = NULL;
    config->run_arg.args_num = 0;
    config->coverage = 1;
    config->run_arg.timeout = 3;
    config->run_arg.fuzz_type = 0;
}

int main(){
    config_t temp ;
    coverage_set_t cover_set;
    init_config(&temp);
    set_configs(&temp);
    fuzzer_main(&temp);
}