#ifndef FUZZER
#define FUZZER

#include <fcntl.h>
#include "./config.h"

void fuzzer_init(config_t* usr_config,run_arg_t* usr_run_arg,input_arg_t* usr_inp_arg);

void fuzzer_main(config_t* usr_config,run_arg_t* usr_run_arg,input_arg_t* usr_inp_arg);

void seed_search(run_arg_t* run_config);
#endif
