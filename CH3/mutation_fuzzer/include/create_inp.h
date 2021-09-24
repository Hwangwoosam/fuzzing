#ifndef FUZZ_INPUT
#define FUZZ_INPUT

#include "./config.h"

void create_inp(char* random_inp,int* inp_size, input_arg_t inp_config);
int delete_random(char* str,char* target,int str_size,int byte);
int insert_random(char* str,char* target,int str_size,int byte);
int random_value(char* str, char* target, int str_size, int byte);
int flip_random(char* str,char* target,int str_size);
int byte_flip(char* str, char* target ,int str_size,int byte);
int simple_arithmatic_insert(char* str,char* target,int str_size,int byte);
int simple_arithmatic_change(char* str,char* target,int str_size,int byte);
int known_integers_insert(char* str, char* target ,int str_size,int byte);
int known_integers_change(char* str, char* target ,int str_size,int byte);
int mutate(char* str,char* target,int str_size);
#endif