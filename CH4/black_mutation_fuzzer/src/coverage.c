#include "../include/coverage.h"
#include "../include/config.h"

void coverage_init(config_t* config,run_arg_t run_arg,coverage_set_t* cover_set){

cover_set->branch_set.input = (char**)malloc(sizeof(char*)*config->trial);

cover_set->branch_set.input_length = (int*)malloc(sizeof(int)*config->trial);

cover_set->branch_set.line_num = (int*)malloc(sizeof(int)*config->trial);

cover_set->branch_set.branch_length = 0;

cover_set->total_excute_line = (int*)malloc(sizeof(int)*run_arg.src_file_num);

cover_set->total_excute_branch =(int*)malloc(sizeof(int)*run_arg.src_file_num);

int src_dir_length = strlen(run_arg.src_dir);

    for(int i = 0; i < run_arg.src_file_num; i++){

        cover_set->total_excute_line[i] = 0;

        cover_set->total_excute_branch[i] = 0;

        char remove_extern[PATH_MAX] = {0,};

        if(get_filename(run_arg.src_file[i],remove_extern) < 0){
            perror("get filename failed\n");
            exit(1);
        }

        int ret = coverage_gcov(remove_extern,run_arg.src_dir);
        
        if(ret < 0){
            perror("coverage gcov failed\n");
            exit(1);
        }

        char* gcov_file = (char*)malloc(sizeof(char)*(strlen(remove_extern) + strlen(run_arg.src_dir) + 9));
        
        sprintf(gcov_file,"%s/%s.c.gcov",run_arg.src_dir,remove_extern);
        
        cover_set->code_size[i] = get_code_line(gcov_file);

        //total line number
        cover_set->line_check[i]= (int*)malloc(sizeof(int)*cover_set->code_size[i][1]);

        //total branch number
        cover_set->branch_check[i] = (int*)malloc(sizeof(int)*cover_set->code_size[i][2]);

        //total execute line check each trial
        cover_set->e_line_check[i] = (int*)malloc(sizeof(int)*config->trial);

        //total excute branch check each trial
        cover_set->e_branch_check[i] = (int*)malloc(sizeof(int)*config->trial);

        memset(cover_set->e_line_check[i],0,sizeof(int)*config->trial);
        memset(cover_set->e_branch_check[i],0,sizeof(int)*config->trial);
        memset(cover_set->line_check[i],0,sizeof(int)*cover_set->code_size[i][1]);
        memset(cover_set->branch_check[i],0,sizeof(int)*cover_set->code_size[i][2]);
        free(gcov_file);
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

int coverage_execute(char* exc_name){

    int pid = fork();
    int status;
    
    if(pid == 0){
        char program[64];
        sprintf(program,"./%s",exc_name);
        char* args[] = {program,NULL}; 
        execv(program,args);

        perror("execute execv error\n");
        exit(1);
    }else if(pid > 0){
        wait(&status);
    }else{
        perror("compile failed\n");
        exit(1);
    }
    return status;
}

int coverage_gcov(char* test_file,char* src_dir){

    int status = 0;
    int pid = fork();

    if(pid == 0){
        char* args[] = {"/usr/bin/gcov","-b","-c",test_file,NULL}; 
        
        chdir(src_dir);
        close(STDOUT_FILENO);
        
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

void read_gcov(coverage_set_t* cover_set,run_arg_t* run_arg,char* random, int random_size,int src_num,int trial){   
    
    char src_file[FILE_NAME_MAX] = {0,};
    
    int ret = get_filename(run_arg->src_file[src_num],src_file);
    
    char* line = NULL;

    char* gcov_file = (char*)malloc(sizeof(char)*(strlen(run_arg->src_dir) + strlen(src_file) + 9));

    sprintf(gcov_file,"%s/%s.c.gcov",run_arg->src_dir,src_file);

    FILE* fp = fopen(gcov_file,"r");
    
    if(fp == NULL){
        
        perror("file open failed\n");
        exit(1);

    }else{

        char* token;
        int e_line=0;
        int e_branch = 0;
        int line_num = 0;

        int n_flag = 0;
               
        size_t len = 0;
        ssize_t s;

        while(!feof(fp)){

            if((s = getline(&line,&len,fp)) > 0){
                
                if(strstr(line,"branch"))
                {
                    if(strstr(line,"take")){
                        token = strtok(line," ");
                        for(int i = 0; i < 3; i ++){
                            token = strtok(NULL," ");
                        }
                        if(atoi(token) > 0){
                            e_branch++;
                            if(cover_set->branch_check[src_num][t_branch] == 0){
                                cover_set->branch_check[src_num][t_branch] = 1;
                                cover_set->total_excute_branch[src_num]++;
                                n_flag = 1;
                            }
                        }
                    }
                    t_branch++;
                }else{

                    token = strtok(line,":");
                    int execution = atoi(token);
                    
                    if(execution > 0){
                        token = strtok(NULL,":");
                        line_num = atoi(token);
                        if(cover_set->line_check[src_num][e_line] == 0){
                            cover_set->line_check[src_num][e_line] = 1;
                            cover_set->total_excute_line[src_num]++;
                            n_flag = 1;
                        }
                        e_line++;
                    }
                }
            }
        }
        fclose(fp);

        if(n_flag == 1){

            int list_length = cover_set->branch_set.branch_length;

            cover_set->branch_set.input[list_length] = (char*)malloc(sizeof(char)*BUFFER_SIZE);
            
            memcpy(cover_set->branch_set.input[list_length],random,random_size);

            cover_set->branch_set.input[list_length][random_size] = '\0';

            cover_set->branch_set.input_length[list_length] = random_size;
            cover_set->branch_set.line_num[list_length] = line_num;
            cover_set->branch_set.branch_length++;
        }

        cover_set->e_branch_check[src_num][trial] = e_branch;
        cover_set->e_line_check[src_num][trial] = e_line;

        free(line);
    }
    free(gcov_file);
}

int* get_code_line(char* path){

    int execute_line = 0;
    int total_branch = 0;
     int num = 0;

    char* line = NULL;
    size_t len = 0;
    int* size = (int*)malloc(sizeof(int)*2);
    char* ptr;

    FILE* fp = fopen(path,"rb");

    if(fp ==NULL){
        perror("get code line file open failed\n");
        exit(1);
    }
    while(!feof(fp) && getline(&line,&len,fp) != 1){
        
        if(strstr(line,":")){
            num++;
        }

        if(strstr(line,"####")){
            execute_line++;
        }else if(strstr(line,"branch")){
            total_branch++;
        }
    }
    
    fclose(fp);
    
    size[0] = execute_line;
    size[1] = num - 4;
    size[2] = total_branch;

    return size;
}


void reset_gcda(run_arg_t run_arg,int src_num){

    int src_file_length = strlen(run_arg.src_file[src_num]);

    char * target_file = (char*)malloc(sizeof(char)*(src_file_length + 1));

    int ret = get_filename(run_arg.src_file[src_num],target_file);

    int src_dir_length = strlen(run_arg.src_dir);

    char* gcda_path = (char*)malloc(sizeof(char)*(src_dir_length + src_file_length + 7));

    sprintf(gcda_path,"%s/%s.gcda",run_arg.src_dir,target_file);
    
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

        free(cover_set->e_line_check[i]);

        free(cover_set->line_check[i]);
        
        free(cover_set->branch_check[i]);
    }

    for(int i = 0; i < cover_set->branch_set.branch_length; i++){
        free(cover_set->branch_set.input[i]);
    }
    free(cover_set->branch_set.line_num);
}