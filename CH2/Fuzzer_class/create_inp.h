#ifndef FUZZ_INPUT
#define FUZZ_INPUT

#include "config.h"

void fuzzer(char* random_inp,int* inp_size, input_arg_t inp_config);

#endif