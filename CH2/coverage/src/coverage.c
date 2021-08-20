#include "../include/coverage.h"
#include "../../Fuzzer_class/include/config.h"

void coverage_init(run_arg_t run_config,coverage_set_t* cover_set,int trial){
    coverage_compile(run_config.src_path,run_config.binary_path);
    cover_set->src_file_name = get_filename(run_config.src_path);
    coverage_gcov(cover_set->src_file_name);
    char gcove_file[64];
    sprintf(gcove_file,"%s.gcov",cover_set->src_file_name);
    cover_set->code_size = get_code_line(gcove_file);

    cover_set->line_check = (int*)malloc(sizeof(int)*cover_set->code_size[1]);
    cover_set->execute_check = (int*)malloc(sizeof(int)*trial);
    memset(cover_set->execute_check,0,sizeof(char)*trial);
    memset(cover_set->line_check,0,sizeof(char)*cover_set->code_size[1]);
}

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

void read_gcov(char* path,int* result,int* list){
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
    char*ptr;
    int num=0;
    while((s = getline(&line,&len,fp)) != -1){
        ptr = strtok(line,":");
        int execution = atoi(ptr);
        if(execution > 0){
            ptr = strtok(NULL,":");
            int line_num = atoi(ptr);
            if(list[line_num] == 0){
                list[line_num]++;
            }
            num++;
        }
    }
    *result = num;
    fclose(fp);
    free(line);
    free(src_file);
}

int* get_code_line(char* path){
    FILE* fp = fopen(path,"rb");
    int num = 0;
    int excute = 0;
    char* line = NULL;
    size_t len = 0;
    int* size = (int*)malloc(sizeof(int)*2);
    while(!feof(fp) && getline(&line,&len,fp) != 1){
        if(strstr(line,"####")){
            excute++;
        }
        num++;
    }
    size[0] = excute;
    size[1] = num;
    return size;
}


void reset_gcda(char* path){
    int path_length = strlen(path);
    char* gcda_path = (char*)malloc(sizeof(char)*(path_length+3));
    sprintf(gcda_path,"./%s.gcda",path);
    if(access(gcda_path,R_OK) == 0){
        if(remove(gcda_path) != 0){
            perror("remove error\n");
        }
    }
}

void coverage_free(coverage_set_t* cover_set){
    free(cover_set->code_size);
    free(cover_set->execute_check);
    free(cover_set->line_check);
    free(cover_set->src_file_name);
    free(cover_set);
}
