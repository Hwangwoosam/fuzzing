#include <stdio.h>
#include <stdlib.h>
#include "../greybox_mutation_fuzzer/include/fuzzer.h"
#include "../greybox_mutation_fuzzer/include/coverage.h"
#include "../greybox_mutation_fuzzer/include/config.h"

void set_configs(config_t* config){
    config->run_arg.binary_path = "./crashme";

    strcpy(config->run_arg.src_dir,"/mnt/c/projects/git/fuzzing/CH4/test");

    char src_file_0[] = "crashme.c";

    config->run_arg.src_file[0] = (char*)malloc(sizeof(char)*(strlen(src_file_0) + 1));
    strcpy(config->run_arg.src_file[0],src_file_0);

    config->run_arg.src_file_num = 1;

    strcpy(config->run_arg.seed_dir,"./seed_dir");
    config->run_arg.seed_file_num = 0;

    config->inp_arg.f_char_start = 32;
    config->inp_arg.f_char_range = 127;

    config->trial = 1000;
    
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_min_len = 100;
    
    config->run_arg.cmd_args = NULL;
    config->run_arg.args_num = 0;
    config->coverage = 0;
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