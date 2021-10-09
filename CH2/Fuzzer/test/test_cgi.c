#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/fuzzer.h"

void set_configs(config_t* config){
    config->run_arg.binary_path = NULL;
    config->run_arg.src_path = "../../cgi_decode/cgi_decode.c";
    config->inp_arg.f_char_start = 32;
    config->inp_arg.f_char_range = 127;
    config->trial = 100;
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_min_len = 100;
    config->run_arg.cmd_args = NULL;
    config->run_arg.args_num = 0;
    config->run_arg.timeout = 3;
    config->run_arg.fuzz_type = 1;
}

int main(){
    config_t temp;
    init_config(&temp);
    set_configs(&temp);
    fuzzer_main(&temp);
}