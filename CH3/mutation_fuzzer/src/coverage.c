#include "../include/coverage.h"
#include "../include/config.h"

void coverage_init(run_arg_t run_config,coverage_set_t* cover_set,int trial){
    coverage_compile(run_config.src_path,run_config.binary_path);
    cover_set->src_file_name = get_filename(run_config.src_path);
    coverage_gcov(cover_set->src_file_name);
    char gcove_file[64];
    sprintf(gcove_file,"%s.gcov",cover_set->src_file_name);
    cover_set->code_size = get_code_line(gcove_file);

    cover_set->line_check = (int*)malloc(sizeof(int)*cover_set->code_size[1]);
    cover_set->branch_check = (int*)malloc(sizeof(int)*cover_set->code_size[2]);
    cover_set->execute_check = (int*)malloc(sizeof(int)*trial);
    cover_set->e_branch_check = (int*)malloc(sizeof(int)*trial);
    cover_set->branch_list = (branch_set_t*)malloc(sizeof(branch_set_t)*cover_set->code_size[2]);

    cover_set->list_length = 0;

    memset(cover_set->execute_check,0,sizeof(char)*trial);
    memset(cover_set->e_branch_check,0,sizeof(char)*trial);
    memset(cover_set->line_check,0,sizeof(char)*cover_set->code_size[1]);
    memset(cover_set->branch_check,0,sizeof(char)*cover_set->code_size[2]);
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
        char* args[] = {"/usr/bin/gcov","-b","-c",test_file,NULL}; 
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

void read_gcov(char* path,int* line_result,int* branch_result,int* list,int* total_branch,branch_set_t* branch_list,int* list_length,char* random, int random_size){
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
    char* ex_ptr,br_ptr;
    int num=0;
    int t_branch = 0;
    int e_branch = 0;
    int line_num = 0;
    int before_branch = 0;
    while((s = getline(&line,&len,fp)) != -1){
        if(strstr(line,":")){
            ex_ptr = strtok(line,":");
            int execution = atoi(ex_ptr);
            if(execution > 0){
                ex_ptr = strtok(NULL,":");
                line_num = atoi(ex_ptr);
                if(list[line_num] == 0){
                    list[line_num]++;
                }
                num++;
            }
        }
        if(strstr(line,"branch")){
            if(strstr(line,"take")){
                if(total_branch[t_branch] == 0){
                    total_branch[t_branch] = 1;
                    if(before_branch != line_num){
                        branch_list[*list_length].branch_num = line_num;
                        branch_list[*list_length].input = (char*)malloc(sizeof(char)*random_size);
                        strcpy(branch_list[*list_length].input,random);
                        *list_length += 1;
                        before_branch = line_num;
                    }                 
                }
                e_branch++;
            }
            t_branch++;
        }
    }
    *line_result = num;
    *branch_result = e_branch;
    fclose(fp);
    free(line);
    free(src_file);
}

int* get_code_line(char* path){
    FILE* fp = fopen(path,"rb");
    int num = 0;
    int excute = 0;
    int t_branch = 0;
    char* line = NULL;
    size_t len = 0;
    int* size = (int*)malloc(sizeof(int)*3);
    char* ptr;
    while(!feof(fp) && getline(&line,&len,fp) != 1){
        if(strstr(line,":")){
            num++;
        }
        if(strstr(line,"####")){
            excute++;
        }
        if(strstr(line,"branch")){
            t_branch++;
        }
    }
    size[0] = excute;
    size[1] = num - 4;
    size[2] = t_branch;
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
    
    free(cover_set->e_branch_check);
    free(cover_set->execute_check);

    free(cover_set->line_check);
    free(cover_set->branch_check);

    free(cover_set->src_file_name);
    for(int i = 0; i < cover_set->list_length; i++){
        free(cover_set->branch_list[i].input);
    }
    free(cover_set->branch_list);
    free(cover_set);
}
