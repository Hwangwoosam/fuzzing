#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * fuzzer(int max_length,int char_start,int char_range){
    
    int string_length = rand()%(max_length+1);
    char* out = (char*)malloc(sizeof(char)*string_length);

    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(char_range) + char_start;
        out[i] = tmp;
    }
    out = realloc(out,string_length+2);
    out[string_length] ='\n';
    out[string_length+1] = '\0';
    return out;
}
