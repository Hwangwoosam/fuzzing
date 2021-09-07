#include "../include/create_inp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_inp(char* random_inp,int * inp_size,input_arg_t inp_config){
    
    int string_length = rand()%(inp_config.f_max_len+1);
    *inp_size = string_length; 
    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(inp_config.f_char_range + 1) + inp_config.f_char_start;
        random_inp[i] = tmp;
    }
    random_inp[string_length] = '\0';

    return;
}


int delete_random(char* str,char* target, int str_size){
    if(str == NULL || str_size <= 0){
        perror("input is NULL");
        return -1;
    }
    int pos = rand()%(str_size);
    if(memcpy(target,str,pos) == NULL){
        perror("delete 1 memcpy failed\n");
        return -1;
    }
    if(memcpy(target + pos, str+pos+1,str_size-pos-1) == NULL){
        perror("delete 2 memcpy failed\n");
        return -1;
    }
    target[str_size-1] ='\0';
    return str_size - 1;
}

int insert_random(char* seed, char* target, int seed_size){
    if(seed == NULL || seed_size <= 0){
        perror("input is NULL");
        return -1;
    }
    int pos = rand()%(seed_size);
    char random_c = rand()%96 + 32;
    if(memcpy(target,seed,pos) == NULL){
        perror("insert 1 memcpy failed\n");
        return -1;
    }
    target[pos] = random_c; 
    if(memcpy(target + pos + 1, seed+pos ,seed_size-pos) == NULL){
        perror("insert 3 memcpy failed\n");
        return -1;
    }
    target[seed_size+1] ='\0';

    return seed_size + 1;
}

int flip_random(char* str, char* target ,int str_size){
    if(str == NULL || str_size <= 0){
        perror("input is NULL");
        return -1;
    }
    int pos = rand()%str_size;
    char c = str[pos];
    char bit = 1 << rand()%7;
    char new_c = c^bit;

    if(memcpy(target,str,pos) == NULL){
        perror("flip 1 memcpy failed\n");
        return -1;
    }
    if(memcpy(target + pos,&new_c,1) == NULL){
        perror("flip 2 memcpy failed\n");
        return -1;
    }
    if(memcpy(target + pos + 1, str+pos + 1,str_size-pos-1) == NULL){
        perror("flip 3 memcpy failed\n");
        return -1;
    }
    target[str_size] ='\0';
    return str_size;
}

int mutate(char* str,char* target,int str_size){

    int random_num = rand()%3;
 
    if(random_num == 0){
       return delete_random(str,target,str_size);

    }else if(random_num == 1){
       return insert_random(str,target,str_size);

    }else{
       return flip_random(str,target,str_size);      
    }


}