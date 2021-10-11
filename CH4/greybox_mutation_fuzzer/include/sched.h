#include "../include/config.h"

typedef struct sched_info{
    int* energy;
    float* nor_energy;
    int list_length;
}sched_info_t;

int sched_init(run_arg_t run_arg,sched_info_t* sched_info);

void assign_energy(sched_info_t* sched_info);

int normalized_energy(sched_info_t* sched_info);

int choose(run_arg_t run_arg,sched_info_t* sched_info);