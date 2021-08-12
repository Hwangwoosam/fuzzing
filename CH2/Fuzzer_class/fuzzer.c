#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include "config.h"
#include "fuzzer.h"

void fuzzer_init(test_config_t* config){

    //#1 path and args check
    int fp_exist = access(config->binary_path,F_OK);
    assert(fp_exist == 0 && "there is no file that exists\n");
    
    if(config->f_max_len < 0){

    }

    if(config->f_min_len < 0){

    }
    //#2 invalid input

    //#3 make dir to store output
    


}

void fuzzer_main(test_config_t* config){
    srand(time(NULL));

    fuzzer_init(config);

    //#2 fuzzer_create_input();

    //#3 fuzzer_run();

    //#4 fuzzer_oracle;
}