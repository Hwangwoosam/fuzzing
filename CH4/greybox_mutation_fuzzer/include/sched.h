#include "../include/config.h"

typedef struct sched_info{
    int* energy;
    float* nor_energy;
    int list_length;
}sched_info_t;

int sched_init(config_t* config,sched_info_t* sched_info);

void assign_energy(sched_info_t* sched_info);

int normalized_energy(sched_info_t* sched_info);

int choose(config_t* config,sched_info_t* sched_info);