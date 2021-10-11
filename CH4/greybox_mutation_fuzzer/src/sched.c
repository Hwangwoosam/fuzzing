#include <stdio.h>
#include <stdlib.h>
#include "../include/sched.h"

int sched_init(run_arg_t run_arg,sched_info_t* sched_info){
    sched_info->list_length = run_arg.seed_file_num;

    sched_info->energy = (int*)malloc(sizeof(int)*sched_info->list_length);
    // sched_info->nor_energy = (float*)malloc(sizeof(float)*sched_info->list_length);
    
    if(sched_info->energy == NULL){
        perror("init energy malloc failed\n");
        return 1;
    }

    // if(sched_info->nor_energy == NULL){
    //     perror("init nor_energy malloc failed\n");
    //     return 1;
    // }

    return 0;
}

void assign_energy(sched_info_t* sched_info){

    for(int i = 0; i < sched_info->list_length; i++){
        sched_info->energy[i] = 1;
    }

}

int normalized_energy(sched_info_t* sched_info){

    int sum = 0;
    for(int i = 0; i < sched_info->list_length; i++){
        sum += sched_info->energy[i];
    }
    // for(int i = 0; i < sched_info->list_length; i++ ){
    //     sched_info->nor_energy[i] = (float)sched_info->energy[i]/(float)sum;
    // }

    return sum;
}

int choose(run_arg_t run_arg,sched_info_t* sched_info){
    sched_init(run_arg,sched_info);
    assign_energy(sched_info);
    
    int sum = normalized_energy(sched_info);
    int r = rand()%sum;
    float cumulate = 0;

    for(int i = 0 ; i < sched_info->list_length; i++){
        cumulate += sched_info->energy[i] ;
        if(r < cumulate){
            return i;
        }
    }

    return 0;
}