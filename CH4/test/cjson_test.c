#include <stdio.h>
#include <stdlib.h>
<<<<<<< HEAD
#include <sys/time.h>
#include <time.h>
=======
>>>>>>> 6136e7fd96e41ca5795b0d9602032d4e1652a254
#include "../greybox_mutation_fuzzer/include/fuzzer.h"
#include "../greybox_mutation_fuzzer/include/coverage.h"
#include "../greybox_mutation_fuzzer/include/config.h"

void set_configs(config_t* config){
    config->run_arg.binary_path = "./cjson/cjson_test";

    strcpy(config->run_arg.src_dir,"/mnt/c/projects/git/fuzzing/CH4/test/cjson");

    char src_file_0[] = "cjson_test.c";
    char src_file_1[] = "cJSON.gcno";

    config->run_arg.src_file[0] = (char*)malloc(sizeof(char)*(strlen(src_file_0) + 1));
    config->run_arg.src_file[1] = (char*)malloc(sizeof(char)*(strlen(src_file_1) + 1));
    strcpy(config->run_arg.src_file[0],src_file_0);
    strcpy(config->run_arg.src_file[1],src_file_1);

    config->run_arg.src_file_num = 2;

    strcpy(config->run_arg.seed_dir,"./seed_dir");
    config->run_arg.seed_file_num = 0;

    config->inp_arg.f_char_start = 32; 
    config->inp_arg.f_char_range = 127;

    config->trial = 5000;
    
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
<<<<<<< HEAD
    time_t start,end;
    init_config(&temp);
    set_configs(&temp);
    
    start = clock();
    // for(int i = 0; i < 10; i++){
        fuzzer_main(&temp);
    // }
    end = clock();
    double excution = (float)(end-start)/CLOCKS_PER_SEC;
    printf("excution time: %f\n",excution);
=======
    init_config(&temp);
    set_configs(&temp);
    fuzzer_main(&temp);
>>>>>>> 6136e7fd96e41ca5795b0d9602032d4e1652a254
}