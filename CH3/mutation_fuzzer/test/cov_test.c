#include <stdio.h>
#include <stdlib.h>
#include "../include/coverage.h"
#include "../include/config.h"


void set_configs(config_t* config){
    config->run_arg.binary_path = "./gcovtest/a.out";
    config->run_arg.src_dir = "/mnt/c/projects/git/fuzzing/CH3/mutation_fuzzer/test/gcovtest";
    config->run_arg.src_file = (char**)malloc(sizeof(char*) * 2);
    config->run_arg.src_file[0] = "flip_random.gcno";
    config->run_arg.src_file[1] = "create_inp.gcno";
    config->run_arg.src_file_num = 2;
    config->run_arg.seed_dir = "./seed_dir";
    config->run_arg.seed_file_num = 0;

    config->inp_arg.f_char_start = 32;
    config->inp_arg.f_char_range = 127;

    config->trial = 10;
    
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_min_len = 100;
    
    config->run_arg.cmd_args = NULL;
    config->run_arg.args_num = 0;
    config->coverage = 1;
    config->run_arg.timeout = 3;
    config->run_arg.fuzz_type = 0;
}

int main(){
    coverage_set_t* cover_set = (coverage_set_t*)malloc(sizeof(coverage_set_t));
    config_t temp ;
    init_config(&temp);
    set_configs(&temp);
    coverage_init(temp.run_arg,cover_set,temp.trial);
    char* src = "/mnt/c/projects/git/fuzzing/CH3/mutation_fuzzer/test/gcovtest/flip_random.gcno";
    printf("%s\n",remove_type(src));
    char *random = "hello world";
    int size = strlen(random);
    read_gcov(cover_set,temp,random,size,0);
    return 0;
}
