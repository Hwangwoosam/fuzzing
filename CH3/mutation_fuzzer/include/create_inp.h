#ifndef FUZZ_INPUT
#define FUZZ_INPUT

#include "./config.h"

void create_inp(char* random_inp,int* inp_size, input_arg_t inp_config);
int delete_random(char* str,char* target,int str_size);
int insert_random(char* str,char* target,int str_size);
int flip_random(char* str,char* target,int str_size);
int mutate(char* str,char* target,int str_size);
#endif