#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/fuzzer.h"
#include "../include/check.h"

void test1(){
    for(int i = 0; i < 100; i++){
        char * data = fuzzer(100,'0',10);
        collapse_if_too_large(data);
    }
}

void test2(){
   for(int i = 0; i < 100; i++){
        char * data = fuzzer(100,'0',10);
         crash_if_too_long(data);
    }
}

void test3(){
    for(int i = 0; i < 100; i++){
        char * data = fuzzer(100,'0',10);
        if(hang_if_no_space(data)){
            printf("there is space\n");
        }else{
            printf("there is no space\n");
        }
    }
}

int main(){
    test1();
    test2();
    test3();
}