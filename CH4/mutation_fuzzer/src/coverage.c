#include "../include/coverage.h"
#include "../include/config.h"

void coverage_init(config_t* config,coverage_set_t* cover_set){
cover_set->src_file_num = config->run_arg.src_file_num;
cover_set->src_file_name = (char**)malloc(sizeof(char*)*config->run_arg.src_file_num);
cover_set->line_check = (int**)malloc(sizeof(int*)*config->run_arg.src_file_num);
cover_set->branch_check = (int**)malloc(sizeof(int*)*config->run_arg.src_file_num);
cover_set->execute_check = (int**)malloc(sizeof(int*)*config->run_arg.src_file_num);
cover_set->e_branch_check = (int**)malloc(sizeof(int*)*config->run_arg.src_file_num);
cover_set->code_size = (int**)malloc(sizeof(int*)*config->run_arg.src_file_num);
cover_set->branch_set = (char**)malloc(sizeof(char*)*config->trial);
cover_set->branch_length = (int*)malloc(sizeof(int));
*cover_set->branch_length = 0;
    for(int i = 0; i < config->run_arg.src_file_num; i++){
        cover_set->src_file_name[i] = (char*)malloc(sizeof(char)*(strlen(config->run_arg.src_file[i]) + 1));
        if(get_filename(config->run_arg.src_file[i],cover_set->src_file_name[i]) < 0){
            perror("get filename failed\n");
            exit(1);
        }
        int ret = coverage_gcov(cover_set->src_file_name[i],config->run_arg.src_dir);
        if(ret < 0){
            perror("coverage gcov failed\n");
            exit(1);
        }
        char gcove_file[256];
        sprintf(gcove_file,"%s/%s.c.gcov",config->run_arg.src_dir,cover_set->src_file_name[i]);
        cover_set->code_size[i] = get_code_line(gcove_file);
        cover_set->line_check[i]= (int*)malloc(sizeof(int)*cover_set->code_size[i][1]);
        cover_set->branch_check[i] = (int*)malloc(sizeof(int)*cover_set->code_size[i][2]);
        cover_set->execute_check[i] = (int*)malloc(sizeof(int)*config->trial);
        cover_set->e_branch_check[i] = (int*)malloc(sizeof(int)*config->trial);

        memset(cover_set->execute_check[i],0,sizeof(int)*config->trial);
        memset(cover_set->e_branch_check[i],0,sizeof(int)*config->trial);
        memset(cover_set->line_check[i],0,sizeof(int)*cover_set->code_size[i][1]);
        memset(cover_set->branch_check[i],0,sizeof(int)*cover_set->code_size[i][2]);
    }
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

int coverage_gcov(char* test_file,char* srd_dir){
    int pid = fork();
    int status;
    if(pid == 0){
        chdir(srd_dir);
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

int get_filename(char* file_name,char* target){
    int path_length = strlen(file_name);
    int idx = 0;
    for(int i = path_length; i >= 0; i--){
        if(file_name[i] == '.'){
            idx = i;
            break;
        }
    }
    if(strncpy(target,file_name,idx) == NULL){
        perror("get filename failed to memcpy\n");
        return -1;
    }
    target[idx] = '\0';
    return 0;
}

void read_gcov(coverage_set_t* cover_set,config_t* config,char* random, int random_size,int src_num,int trial){   
    char src_file[64];
    memset(src_file,0,sizeof(char)*64);
    int ret = get_filename(config->run_arg.src_file[src_num],src_file);
    char* line = NULL;
    char token[1024];
    char token2[1024];
    size_t len = 0;
    ssize_t s;
    char gcove_file[256];
    sprintf(gcove_file,"%s/%s.c.gcov",config->run_arg.src_dir,src_file);
    FILE* fp = fopen(gcove_file,"r");
    if(fp == NULL){
        perror("file open failed\n");
        exit(1);
    }else{
        char* ex_ptr;
        int num=0;
        int t_branch = 0;
        int e_branch = 0;
        int line_num = 0;

        int n_flag = 0;
        while(!feof(fp)){
            if((s = getline(&line,&len,fp)) != -1){
                memset(token,0,1024);
                memset(token2,0,1024);
                strcpy(token,line);
                strcpy(token2,line);
                if(strstr(token,":")){
                    ex_ptr = strtok(token,":");
                    int execution = atoi(ex_ptr);
                    if(execution > 0){
                        ex_ptr = strtok(NULL,":");
                        line_num = atoi(ex_ptr);
                        if(cover_set->line_check[src_num][line_num] == 0){
                            cover_set->line_check[src_num][line_num] = 1;
                            n_flag = 1;
                        }
                        num++;
                    }
                }else{
                    if(strstr(token2,"branch")){
                        if(strstr(token2,"take")){
                            if(cover_set->branch_check[src_num][t_branch] == 0){
                                cover_set->branch_check[src_num][t_branch] = 1;
                                n_flag = 1;
                            }
                            e_branch++;
                        }
                        t_branch++;
                    }
                }
            }
        }
        if(n_flag == 1){
            int list_length = cover_set->branch_length[0];
            cover_set->branch_set[list_length] = (char*)malloc(sizeof(char)*random_size+1);
            strcpy(cover_set->branch_set[list_length],random);
            cover_set->branch_set[list_length][random_size] = '\0';
            cover_set->branch_length[0]++;
        }
        cover_set->e_branch_check[src_num][trial] = num;
        cover_set->execute_check[src_num][trial] = e_branch;   
        free(line);
        fclose(fp);
    }
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


void reset_gcda(config_t config,int src_num){
    char * target_file = (char*)malloc(sizeof(char)*(strlen(config.run_arg.src_file[src_num]) + 1));
    int ret = get_filename(config.run_arg.src_file[src_num],target_file);
    char gcda_path[256];
    sprintf(gcda_path,"%s/%s.gcda",config.run_arg.src_dir,target_file);
    if(access(gcda_path,R_OK) == 0){
        if(remove(gcda_path) != 0){
            perror("remove error\n");
        }
    }
}

void coverage_free(coverage_set_t* cover_set,int num){
    for(int i = 0; i < num ; i++){
        free(cover_set->code_size[i]);
        free(cover_set->e_branch_check[i]);
        free(cover_set->execute_check[i]);
        free(cover_set->line_check[i]);
        free(cover_set->branch_check[i]);
        free(cover_set->src_file_name[i]);
    }
    free(cover_set->code_size);
    free(cover_set->e_branch_check);
    free(cover_set->execute_check);
    free(cover_set->line_check);
    free(cover_set->branch_check);
    free(cover_set->src_file_name);
}