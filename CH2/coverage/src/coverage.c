#include "../include/coverage.h"


int coverage_compile(char *test_file,char* exc_file){
    int pid = fork();
    int status;
    if(pid == 0){
        char* args[] = {"/usr/bin/gcc","--coverage","-o",exc_file,test_file,NULL}; 
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

char* get_filename(char* path){
    int path_length = strlen(path);
    int idx = 0;
    for(int i = path_length; i >= 0; i--){
        if(path[i] == '/'){
            idx = i;
            break;
        }
    }
    char* cpy_file_name = (char*)malloc(sizeof(char)*(path_length+1));
    memcpy(cpy_file_name,path+idx+1,path_length-idx+1);
    return cpy_file_name;
}

void make_gcov(int argc,char* argv[]){
    char* src_filename = get_filename(argv[1]);
    
    int file_length = strlen(src_filename);
    char* exc_file = (char*)malloc(sizeof(char)*file_length);
    memcpy(exc_file,src_filename,file_length-2);
    char** args;
    if(argc == 2){
        args = (char**)malloc(sizeof(char*)*argc);
        args[0] = exc_file;
        args[1] = NULL;
    }else if(argc > 2){
        args = (char**)malloc(sizeof(char*)*argc);
        args[0] = exc_file;
        for(int i = 0; i < argc-2 ; i++){
            args[i+1] = argv[i+2];
        }
    }
    args[argc-1] = NULL; 
    coverage_compile(argv[1],args[0]);
    coverage_excute(args[0]); 
    coverage_gcov(src_filename);
    free(args);
    free(src_filename);
}

void read_gcov(char* path){
    char* src_file = get_filename(path);
    FILE* fp;
    char* line = (char*)malloc(sizeof(char)*256) ;
    size_t len = 256;
    int s;
    char gcove_file[64];
    sprintf(gcove_file,"%s.gcov",src_file);
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
    free(src_file);
}
