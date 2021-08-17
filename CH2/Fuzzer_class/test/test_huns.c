#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/fuzzer.h"

void set_configs(test_config_t* config){
    strcpy(config->run_arg.binary_path,"/usr/local/bin/hunspell");
    char* test_arg[] = {"/usr/local/bin/hunspell",NULL};
    config->inp_arg.f_char_start = 0;
    config->inp_arg.f_char_range = 255;
    config->trial = 1000;
    config->inp_arg.f_max_len = 100;
    config->inp_arg.f_min_len = 100;
    config->run_arg.cmd_args = test_arg;
    config->run_arg.cmd_args = 0;
}

int main(){
    test_config_t temp;
    init_config(&temp);
    set_configs(&temp);
    fuzzer_main(&temp);
}