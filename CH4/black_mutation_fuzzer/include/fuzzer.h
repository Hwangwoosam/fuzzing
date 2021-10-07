#ifndef FUZZER
#define FUZZER

#include <fcntl.h>
#include "./config.h"

void fuzzer_init(config_t* );

void fuzzer_main(config_t* );

void seed_search(run_arg_t* run_config);
#endif
