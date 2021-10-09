#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/simple_fuzzer.h"

//Buffer overflow
void crash_if_too_long(char* s){
    char buffer[] = "Thursday";
    if(strlen(s) > strlen(buffer)){
        perror("Too Long\n");
    }
}

//Missing Error Checks
int hang_if_no_space(char* s){
    int i = 0;
    time_t start,end;
    start = clock();
    while(1){
        if(s[i] == ' '){
            return 1;
        }
        end = clock();
        if((double)(end-start)/CLOCKS_PER_SEC > 2){
            break;
        }
    }
    return 0;
}

// Rogue Numbers
void collapse_if_too_large(char* s){
    int s_num =atoi(s);
    printf("%d\n",s_num);

    if(s_num > 1000 || s_num < 0){
        perror("Too Large number\n");
    }
}