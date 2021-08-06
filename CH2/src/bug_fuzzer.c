#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/fuzzer.h"


void crash_if_too_long(char* s){
    char buffer[] = "Thursday";
    if(strlen(s) > strlen(buffer)){
        perror("Too Long\n");
    }
}

int hang_if_no_space(char* s){
    int i = 0;
    while(1){
        if(s[i] == ' '){
            return 1;
        }
    }
    return 0;
}

void collapse_if_too_large(char*s){
    int s_num =atoi(s);
    if(s_num > 1000){
        perror("Too Large number\n");
    }
}