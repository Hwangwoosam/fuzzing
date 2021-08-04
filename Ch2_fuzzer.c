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

char template[] = "tmp.XXXXXX";
char *dir_name;

typedef struct file_info{
    char* data;
    int length;
}file_info_t;

char * fuzzer(int max_length,int char_start,int char_range,int * length){
    int string_length = rand()%(max_length+1);
    char* out = (char*)malloc(sizeof(char)*string_length);

    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(char_range) + char_start;
        out[i] = tmp;
    }
    *length = string_length;

    return out;
}

void fuzzer_test(){
    int length = 0;
    char * tmp = fuzzer(1000,'a',26,&length);
    printf("%s %d\n",tmp,length);

    tmp = fuzzer(1000,-1,135,&length);
    printf("%s %d\n",tmp,length);
    free(tmp);
}

int main(){
    //Test
    srand(time(NULL));
    fuzzer_test();
}