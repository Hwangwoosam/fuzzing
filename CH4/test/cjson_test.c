#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include "../greybox_mutation_fuzzer/include/fuzzer.h"
#include "../greybox_mutation_fuzzer/include/coverage.h"
#include "../greybox_mutation_fuzzer/include/config.h"

void set_configs(config_t* config,run_arg_t* run_arg,input_arg_t* inp_arg){
    run_arg->binary_path = "./cjson/cjson_test";

    strcpy(run_arg->src_dir,"/mnt/c/projects/git/fuzzing/CH4/test/cjson");

    char src_file_0[] = "cjson_test.c";
    char src_file_1[] = "cJSON.gcno";

    run_arg->src_file[0] = (char*)malloc(sizeof(char)*(strlen(src_file_0) + 1));
    run_arg->src_file[1] = (char*)malloc(sizeof(char)*(strlen(src_file_1) + 1));
    strcpy(run_arg->src_file[0],src_file_0);
    strcpy(run_arg->src_file[1],src_file_1);

    run_arg->src_file_num = 2;

    strcpy(run_arg->seed_dir,"./seed_dir");
    run_arg->seed_file_num = 0;

    inp_arg->f_char_start = 32; 
    inp_arg->f_char_range = 127;

    config->trial = 5000;
    
    inp_arg->f_max_len = 100;
    inp_arg->f_min_len = 100;
    
    run_arg->cmd_args = NULL;
    run_arg->args_num = 0;
    config->coverage = 1;
    run_arg->timeout = 3;
    run_arg->fuzz_type = 0;
}

int main(){
    config_t config ;
    run_arg_t run_arg;
    input_arg_t inp_arg;
    coverage_set_t cover_set;
    time_t start,end;
    init_config(&config,&run_arg,&inp_arg);
    set_configs(&config,&run_arg,&inp_arg);
    
    start = clock();
    // for(int i = 0; i < 10; i++){
        fuzzer_main(&config,&run_arg,&inp_arg);
    // }
    end = clock();
    double excution = (float)(end-start)/CLOCKS_PER_SEC;
    printf("excution time: %f\n",excution);
}