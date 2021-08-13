#ifndef FUZZER
#define FUZZER

#include <fcntl.h>
#include "config.h"

void fuzzer_init(test_config_t* );

void fuzzer_main(test_config_t* );

#endif
