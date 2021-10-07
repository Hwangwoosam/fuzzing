#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../mutation_fuzzer/include/fuzzer.h"
#include "../mutation_fuzzer/include/sched.h"

void set_configs(config_t* config){
    config->run_arg.seed_dir = "./seed_dir";
    config->run_arg.seed_file_num = 0;
}

int main(){
    srand(time(NULL));
    config_t temp ;
    sched_info_t sched_info;
    init_config(&temp);
    set_configs(&temp);
    seed_search(&temp.run_arg);
    sched_init(&temp,&sched_info);
    int array[8] = {0,};
    for(int i = 0; i < 10000; i++){
        int idx = choose(&sched_info);
        array[idx]++;
    }
    for(int i = 0; i < 8; i++){
        printf("%0.3f ",(float)array[i]/(float)10000);
    }
    printf("\n");
}