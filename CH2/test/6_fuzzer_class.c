#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "../include/0_fuzzer.h"
#include "../include/5_program_runner.h"
#include "../include/6_fuzzer_class.h"


int main(){

    srand(time(NULL));
    fuzzer_t* temp = init_class(100,10,32,32);
    char program[]="cat";
    run_class(program,temp,10);
    free(temp);
    
    return 0;
}