#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

char * fuzzer(int max_length,int char_start,int char_range){
    
    int string_length = rand()%(max_length+1);
    char* out = (char*)malloc(sizeof(char)*string_length);

    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(char_range) + char_start;
        out[i] = tmp;
    }
    out[string_length-1] ='\0';
    return out;
}

void fuzzer_test(){
    char * tmp = fuzzer(100,32,32);
    printf("%s\n",tmp);

    // tmp = fuzzer(1000,-500,500,&length);
    // printf("%s %d\n",tmp,length);
    // free(tmp); 
}

int main(){
    //Test
    srand(time(NULL));
    for(int i = 0; i < 100; i++){
     fuzzer_test();    
    }

}