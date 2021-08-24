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

char* delete_random(char* str){
    if(str == ""){
        perror("input is NULL");
        return NULL;
    }
    int length = strlen(str);
    char* change = (char*)malloc(sizeof(char)*length);
    int pos = rand()%length;
    memcpy(change,str,pos);
    memcpy(change + pos, str+pos+1,length-pos-1);
    return change;
}

char* insert_random(char* str){
    int length = strlen(str);
    char* change = (char*)malloc(sizeof(char)*(length+1));
    int pos = rand()%length;
    char random_c = rand()%95 + 32;
    memcpy(change,str,pos);
    memcpy(change + pos,&random_c,1);
    memcpy(change + pos + 1, str+pos,length-pos);
    return change;
}

char* flip_random(char* str){
    if(str == ""){
        perror("input is NULL");
        return NULL;
    }
    int length = strlen(str);
    char* change = (char*)malloc(sizeof(char)*(length+1));
    int pos = rand()%length;
    char c = str[pos];
    char bit = 1<< rand()%6;
    char new_c = c^bit;
    memcpy(change,str,pos);
    memcpy(change + pos,&new_c,1);
    memcpy(change + pos + 1, str+pos,length-pos-1);
    return change;
}

char* mutate(char* str){

    int random_num = rand()%3;
    if(random_num == 0){
        char* delete_r = delete_random(str);

        return delete_r;
    }else if(random_num == 1){
        char* insert_r = insert_random(str);

        return insert_r;
    }else{
        char* flip_r = flip_random(str);

        return flip_r;
    }
}