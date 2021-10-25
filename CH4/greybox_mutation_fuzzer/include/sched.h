#include "../include/config.h"

typedef struct sched_info{
    float* energy;
    float sum;
    int list_length;
    int current_idx;
    int path_fre[NUM_OF_MAX];
}sched_info_t;

int sched_init(run_arg_t* run_arg,sched_info_t* sched_info);

void assign_energy(sched_info_t* sched_info);

int choose(run_arg_t run_arg,sched_info_t* sched_info);