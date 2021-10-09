#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/fuzzer.h"

void set_configs(config_t* config){
    config->run_arg.binary_path = "../../../CH2/Troff/s_troff";
    config->run_arg.src_path = "../../../CH2/Troff/small_troff.c";
    config->run_arg.seed_dir = "./seed_dir";
    config->inp_arg.f_char_start = 0;
    config->inp_arg.f_char_range = 255;

    config->trial = 100;
    
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_min_len = 100;
    config->run_arg.cmd_args = NULL;
    config->run_arg.cmd_args = 0;
    config->coverage = 1;
}

int main(){
    config_t temp;
    init_config(&temp);
    set_configs(&temp);
    fuzzer_main(&temp);
}