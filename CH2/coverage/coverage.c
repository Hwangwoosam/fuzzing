#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <execinfo.h>
#include <unistd.h>
#include <sys/wait.h>


int coverage_compile(char *test_file,char* exc_file){
    int pid = fork();
    int status;
    if(pid == 0){
        char* args[] = {"/usr/bin/gcc","-fprofile-arcs","-ftest-coverage","-o",exc_file,test_file,NULL}; 
        execv("/usr/bin/gcc",args);
        perror("compile execv error\n");
        exit(1);
    }else if(pid > 0){
        wait(&status);
        printf("compile complete\n");
    }else{
        perror("compile failed\n");
        exit(1);
    }

    return status;
} 

int coverage_excute(char* exc_name){

    int pid = fork();
    int status;
    
    if(pid == 0){
        char program[64];
        sprintf(program,"./%s",exc_name);
        char* args[] = {program,NULL}; 
        execv(program,args);

        perror("excute execv error\n");
        exit(1);
    }else if(pid > 0){
        wait(&status);
        printf("excute complete\n");
    }else{
        perror("compile failed\n");
        exit(1);
    }
    return status;
}

int coverage_gcov(char* test_file){
    int pid = fork();
    int status;
    if(pid == 0){
        char* args[] = {"/usr/bin/gcov",test_file,NULL}; 
        execv("/usr/bin/gcov",args);

        perror("gcov execv error\n");
        exit(1);
    }else if(pid > 0){
        wait(&status);
        printf("gcov complete\n");
    }else{
        perror("compile failed\n");
        exit(1);
    }
    return status;
}

void make_gcov(char* test_file){
    pid_t comp_child,exc_child,res_child;

    char* cpy_file_name = (char*)malloc(sizeof(char)*(strlen(test_file)+1));
    strcpy(cpy_file_name,test_file);
    char* exc_file = strtok(cpy_file_name,".");
    
    coverage_compile(test_file,exc_file);
    coverage_excute(exc_file); 
    coverage_gcov(test_file);

    free(cpy_file_name);
}

void read_gcov(char* test_file){
    FILE* fp;
    char* line = (char*)malloc(sizeof(char)*256) ;
    size_t len = 256;
    int s;
    char gcove_file[64];
    sprintf(gcove_file,"%s.gcov",test_file);
    fp = fopen(gcove_file,"r");
    if(fp == NULL){
        perror("file open failed\n");
        exit(1);
    }
    while((s = getline(&line,&len,fp)) != -1){
        char*ptr = strtok(line,":");
        int execution = atoi(ptr);
        if(execution > 0){
            printf("execute: %d : ",atoi(ptr));
            ptr = strtok(NULL,":");
            int line_num = atoi(ptr);
            printf("%d\n",atoi(ptr));
        }
    }
    fclose(fp);
    free(line);
}

int main(){
    char text[15]= "a+b";
    char test_file[] = "ascending.c";
    make_gcov(test_file);
    read_gcov(test_file);
    return 0;
}