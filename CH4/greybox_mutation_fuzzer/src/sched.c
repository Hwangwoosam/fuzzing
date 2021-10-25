#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/sched.h"

int sched_init(run_arg_t* run_arg,sched_info_t* sched_info){
    sched_info->list_length = run_arg->seed_file_num;
    sched_info->current_idx = 0;
    sched_info->sum = 0 ;
    sched_info->energy = (float*)malloc(sizeof(float)*NUM_OF_MAX);
    
    for(int i = 0; i < NUM_OF_MAX; i++){
        sched_info->path_fre[i] = 1;
    }

    if(sched_info->energy == NULL){
        perror("init energy malloc failed\n");
        return 1;
    }

    return 0;
}

void assign_energy(sched_info_t* sched_info){

    for(int i = 0; i < sched_info->list_length; i++){

        sched_info->energy[i] = (float)1/(float)pow(sched_info->path_fre[i],0.7);
        sched_info->sum += sched_info->energy[i];
    }

}

int choose(run_arg_t run_arg,sched_info_t* sched_info){
     assign_energy(sched_info);
    int r = rand()%100;
    float cumulate = 0;

    for(int i = 0 ; i < sched_info->list_length; i++){
        cumulate += (sched_info->energy[i]/sched_info->sum)*100;
        if(r < cumulate){
            return i;
        }
    }

    return 0;
}