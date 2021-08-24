#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/urlparse.h"
#include "../../../CH2/Fuzzer_class/include/config.h"
#include "../../../CH2/Fuzzer_class/include/create_inp.h"

void test_function(){
    char str[] = "http://www.google.com/search?q=fuzzing";
    int status = http_program(str);
    printf("%d\n",status);
}

void test_random_inp(){
    input_arg_t inp_config;
    inp_config.f_max_len = 70;
    inp_config.f_min_len = 70;
    inp_config.f_char_start = 32;
    inp_config.f_char_range = 96;

    char* random_inp = (char*)malloc(sizeof(char*)*(inp_config.f_max_len + 1));
    for(int i = 0; i < 1000;i++){
        memset(random_inp,0,inp_config.f_max_len+1);
        int rand_size = 0;
        create_inp(random_inp,&rand_size,inp_config);
        url_t* result;
        result = url_parse(random_inp);
        if(result == NULL){
            printf("Failed and continue\n");
        }else{
            free_url(result);
        }
    }
    free(random_inp);
}

void test_mutation(){
    char seed_input[] = "http://www.google.com/search?q=fuzzing";
    char** valid_inp = (char**)malloc(sizeof(char)* 20);
    int idx = 0;
    for(int i = 0; i < 20; i++){
        char* inp = mutate(seed_input);
        if(is_valid_url(inp) == 1){
            int length = strlen(inp);
            valid_inp[idx] = (char*)malloc(sizeof(char)*(length+1));
            memcpy(valid_inp[idx],inp,length);
            valid_inp[idx][length] ='\0';
            idx++;
        }
        free(inp);
    }
    printf("=========================\n");
    for(int i = 0; i < idx; i++){
        printf("%s %d\n",valid_inp[i],i);
        free(valid_inp[i]);
    }
}
int main(){
    srand(time(NULL));
    test_mutation();
    return 0;
}