#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <signal.h>
#include <sys/time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#include "../include/create_inp.h"
#include "../include/fuzzer.h"
#include "../include/coverage.h"
#include "../include/sched.h"

static config_t test_config;
static input_arg_t inp_arg;
static run_arg_t run_arg;

static pid_t child;

static int (* oracle) (char * dir_name,int trial,int ret_code) ;

static char tmp_dir_name[20];
static int failed;
static int passed;
static int unresol;

static int pipe_stdin[2];
static int pipe_stdout[2];
static int pipe_stderr[2];

void create_dir(){
    char template[] = "tmp.XXXXXX" ;
    char * temp_dir = mkdtemp(template) ;
    if (temp_dir == 0x0) {
        perror("mkdtemp") ;
        exit(1) ;
    }
    strcpy(tmp_dir_name, temp_dir) ;
}

void timeout_handler(int sig){
    if(sig == SIGALRM){
        perror("Time out\n");
        if(kill(child,SIGINT) == -1){
            perror("failed to kill child process\n");
            exit(1);
        }
    }
}

void save_data(char* filename,int pipe){
 
    char buf[BUFFER_SIZE];
    
    int fd = open(filename,O_RDWR | O_TRUNC | O_CREAT , 0600);
    
    if(fd < 0){
        perror("output fd open failed\n");
        exit(1);
    }

    int total = 0;
    int sent = 0;


    while((total=read(pipe,buf,BUFFER_SIZE)) > 0){
 
        if(total > 0){
            sent = 0;
            while(sent < total){
                sent += write(fd,buf,total-sent);
            }
            total = 0;
        }

        if(memset(buf,0,BUFFER_SIZE) == NULL){
            perror("save data memset failed\n");
        }
    }
    
    close(pipe);
    
    if(close(fd) == -1 ){
        perror("output fd close failed\n");
        exit(1);
    }

}

void write_result(char* inp, int inp_size,int order){
     
    close(pipe_stdout[1]);
    close(pipe_stderr[1]);
    close(pipe_stdin[1]);
    close(pipe_stdin[0]);

    char buf[BUFFER_SIZE];

    char out_fpath[PATH_MAX];
    char err_fpath[PATH_MAX];
    char inp_fpath[PATH_MAX];

    sprintf(inp_fpath,"%s/%d_input",tmp_dir_name,order);
    sprintf(out_fpath,"%s/%d_output",tmp_dir_name,order);
    sprintf(err_fpath,"%s/%d_error",tmp_dir_name,order);

    int fd = open(inp_fpath, O_RDWR | O_TRUNC | O_CREAT , 0600);

    if(fd < 0){
        perror("input fd open failed\n");
        exit(1);
    }
    int sent = 0;

    while(sent < inp_size){
        sent += write(fd,inp+sent,inp_size-sent);
        if(sent == -1){
            perror("input write error\n");
            exit(1);
        }
    }

    if(close(fd) == -1 ){
        perror("input fd close failed\n");
        exit(1);
    }


    save_data(out_fpath,pipe_stdout[0]);        
    save_data(err_fpath,pipe_stderr[0]);

}

void execute_target(char* inp, int inp_size){
    dup2(pipe_stdin[0],0);
    close(pipe_stdin[1]);
    close(pipe_stdin[0]);
    close(pipe_stdout[0]);
    close(pipe_stderr[0]);

    dup2(pipe_stdout[1],STDOUT_FILENO);
    dup2(pipe_stderr[1],STDERR_FILENO);

    close(pipe_stderr[1]);
    close(pipe_stdout[1]);

    execv(run_arg.binary_path,run_arg.cmd_args);
    perror("execv error\n");
    exit(1);
}

int run(run_arg_t run_config ,char* random_inp,int inp_size,int order){

    if(pipe(pipe_stdin) != 0) {
        perror("stdin pipe error\n") ;
        exit(1) ;
    }
    if (pipe(pipe_stdout) != 0) {
        perror("stdout pipe error\n") ;
        exit(1) ;
    }

    if (pipe(pipe_stderr) != 0) {
        perror("stderr pipe error\n") ;
        exit(1) ;
    }

    int status;
    child = fork();
    
    if(child > 0){

        write_result(random_inp,inp_size,order);
    
    }else if(child == 0){
        
        int sent = 0;
        
        while(sent < inp_size)
        {
            sent += write(pipe_stdin[1],random_inp + sent,inp_size - sent);
        }

        alarm(run_config.timeout);
        execute_target(random_inp,inp_size);

    }else{
        perror("fork failed\n");
        exit(1);
    }
    wait(&status);

    return status;
}

void seed_search(run_arg_t* run_config){
    DIR * dp;
    struct dirent * ep;
    dp = opendir(run_config->seed_dir);
    int idx = 0;
    char*path = (char*)malloc(sizeof(char) *PATH_MAX);

    int seed_dir_length = strlen(run_config->seed_dir);

    while(ep = readdir(dp)){
        if(ep->d_type == DT_REG){

            int name_length = strlen(ep->d_name);

            sprintf(path,"%s/%s",run_config->seed_dir,ep->d_name);

            char buf[BUFFER_SIZE];
    
            FILE* fp = fopen(path,"rb");

            int total_len = 0;
            int sent = 0;

            run_config->seed_inp[idx] = (char*)malloc(sizeof(char)* BUFFER_SIZE);

            while((sent = fread(buf,sizeof(char),BUFFER_SIZE,fp)) > 0){
                if(total_len + sent > BUFFER_SIZE){
                char* check = realloc(run_config->seed_inp[idx],sizeof(char)*((total_len/BUFFER_SIZE)+1)*BUFFER_SIZE);
                    if(check == NULL){
                        perror("failed to realloc\n");
                        exit(0);
                    }        
                }

                if(memcpy(run_config->seed_inp[idx] + total_len,buf,sent) == NULL){
                    perror("seed_read memcpy failed\n");
                    return;
                }

                if(memset(buf,0,BUFFER_SIZE) == NULL){
                    perror("buf memset failed\n");
                }

                total_len += sent;
            }

            fclose(fp);
            
            if(total_len == 0)
            {
                perror("No seed_input\n");
                return;
            }else
            {
                run_config->seed_inp[idx][total_len] ='\0';
                run_config->seed_length[idx] = total_len;
                idx++;

                if(memset(path,0,BUFFER_SIZE) == NULL){
                    perror("path memset failed\n");
                }
            }
        }
    }
    run_config->seed_file_num = idx;

    free(path);
    closedir(dp);
}

void fuzzer_init(config_t* usr_config,run_arg_t* usr_run_arg,input_arg_t* usr_inp_arg){

    //src directory copy
    if(usr_run_arg->src_dir == NULL){

        perror("there is no src directory\n");
        exit(1);

    }else{

        int src_dir_exist = access(usr_run_arg->src_dir,00);
        assert(src_dir_exist == 0 && "this dirtory doesn't exist or can't excute\n");

        strcpy(run_arg.src_dir,usr_run_arg->src_dir);
    }

    //all src file name copy
    if(usr_run_arg->src_file_num > 0){

        run_arg.src_file_num = usr_run_arg->src_file_num; 

        int length = strlen(usr_run_arg->src_dir);

        char* file_checker = (char*)malloc(sizeof(char)*PATH_MAX);
        for(int i = 0; i < run_arg.src_file_num; i++){

            sprintf(file_checker,"%s/%s",usr_run_arg->src_dir,usr_run_arg->src_file[i]);
            
            int src_exist = access(file_checker,F_OK);
            assert(src_exist == 0 && "this file doesn't exist or can't read\n");

             int src_file_length = strlen(usr_run_arg->src_file[i]);

            run_arg.src_file[i] = (char*)malloc(sizeof(char)*(length + 1));
            strcpy(run_arg.src_file[i],usr_run_arg->src_file[i]);

        }
        free(file_checker);
    }
    
    int binary_length = strlen(usr_run_arg->binary_path);

    //binary path copy
    if(usr_run_arg->binary_path != NULL){

        if(binary_length > PATH_MAX){

            perror("binary path length is too long\n");
            exit(1);

        }else{
            int binary_exist = access(usr_run_arg->binary_path,X_OK);
            assert(binary_exist == 0 && "this file doesn't exist or can't excute\n");

            run_arg.binary_path = (char*)malloc(sizeof(char)*(binary_length+1));

            strcpy(run_arg.binary_path,usr_run_arg->binary_path);
        }
    }

    //seed dirtory copy
    if(usr_run_arg->seed_dir != NULL){

        if(strlen(usr_run_arg->seed_dir) > PATH_MAX){

            perror("src path length is too long\n");
            exit(1);

        }else{

            int seed_exist = access(usr_run_arg->seed_dir,00);
            assert(seed_exist == 0 && "this dirtory doesn't exist or can't excute\n");

            strcpy(run_arg.seed_dir,usr_run_arg->seed_dir);
            
            //get seed
            seed_search(&run_arg);
        }
    }

    inp_arg.f_min_len = usr_inp_arg->f_min_len;
    inp_arg.f_max_len = usr_inp_arg->f_max_len;
    inp_arg.f_char_start = usr_inp_arg->f_char_start;
    inp_arg.f_char_range = usr_inp_arg->f_char_range;
    
    run_arg.fuzz_type = usr_run_arg->fuzz_type;
    run_arg.args_num = usr_run_arg->args_num;

    if(run_arg.fuzz_type == 1){
        run_arg.cmd_args = (char**)malloc(sizeof(char*)*(run_arg.args_num + 3));
        run_arg.cmd_args[run_arg.args_num+2] = NULL; 
    }else{
        run_arg.cmd_args = (char**)malloc(sizeof(char*)*(run_arg.args_num + 2));
        run_arg.cmd_args[run_arg.args_num+1] = NULL;
    }
    run_arg.cmd_args[0] = (char*)malloc(sizeof(char)*(binary_length+1));
    strcpy(run_arg.cmd_args[0],run_arg.binary_path);
   
    if(run_arg.args_num > 0){

        for(int i = 1 ; i < run_arg.args_num + 1; i++){
            run_arg.cmd_args[i] = (char*)malloc(sizeof(char)*(strlen(run_arg.cmd_args[i-1] + 1)));
            strcpy(run_arg.cmd_args[i],run_arg.cmd_args[i-1]);
        }

    }else if(run_arg.args_num < 0){

        perror("Wrong arg size\n");
        exit(1);
    }

    run_arg.timeout = usr_run_arg->timeout;
    test_config.coverage = usr_config->coverage;
    test_config.trial = usr_config->trial;
    test_config.oracle = usr_config->oracle;
    
    if(inp_arg.f_min_len < 0){
        perror("Wrong min length\n");
        exit(1);
    }

    if(inp_arg.f_max_len < 0){
        perror("Wrong max length\n");
        exit(1);
    }

    if(inp_arg.f_min_len > inp_arg.f_max_len){
        perror("min length is bigger than max length\n");
        exit(1);
    }

    if(inp_arg.f_char_start + inp_arg.f_char_range > 255 ){
        perror("out of character range\n");
        exit(1);
    }

    if(inp_arg.f_char_start < 0 || inp_arg.f_char_range < 0){
        perror("char_start or char_range is not negative\n");
        exit(1);
    }

    //#3 make dir to store output
    create_dir();
}

void delete_result(){
    
    char inp[256];
    char out[256];
    char err[256];
    
    for(int i = 0; i < test_config.trial; i++){

        sprintf(inp,"%s/%d_input",tmp_dir_name,i);
        sprintf(out,"%s/%d_output",tmp_dir_name,i);
        sprintf(err,"%s/%d_error",tmp_dir_name,i);

        remove(inp);
        remove(out);
        remove(err);
    }

    if(rmdir(tmp_dir_name) == -1){
        perror("rmdir failed: ");
        exit(0);
    }
}

void config_free(run_arg_t* run, input_arg_t* inp){

    if(run->src_file_num > 0){
        for(int i = 0; i < run->src_file_num;i++){
            free(run->src_file[i]);
        }
    }

    if(run->binary_path != NULL){
        free(run->binary_path); 
    }

    for(int i = 0; i < run->seed_file_num;i++){
        free(run->seed_inp[i]);
    }

    if(run->fuzz_type == 1){
        run->args_num += 3;
    }else{
        run->args_num += 2;
    }

    for(int i = 0; i < run->args_num; i++){

        free(run->cmd_args[i]); 
    }

    free(run->cmd_args);
}

char* create_candidate(run_arg_t run_arg,sched_info_t* sched_info,int* inp_size){
    
    int seed_idx = choose(run_arg,sched_info);

    char* candidate = (char*)malloc(sizeof(char)*BUFFER_SIZE);

    if(memcpy(candidate,run_arg.seed_inp[seed_idx], run_arg.seed_length[seed_idx]) == NULL){
        perror("seed copy failed\n");
        exit(1);
    }

    candidate[run_arg.seed_length[seed_idx]-1] = '\0';

    // int trial = 1 << rand()%5 + 1;
    
    int candidate_length = strlen(candidate);
    
    // if(candidate_length < trial){
        // trial = candidate_length;
    // }

    int length = candidate_length;

    char buf[BUFFER_SIZE];

    if((length = mutate(candidate,buf,length))<= 0){
        return candidate;
    }
    // for(int i = 0; i <trial ; i++){
    //    if((length = mutate(candidate,buf,length))<= 0){
    //     //    printf("error\n");
    //        return candidate;
    //    }else{
    //        memcpy(candidate,buf,length);
    //        candidate[length - 1]='\0';
    //    }
    // }
    *inp_size = length;

    return candidate;
}

void sellect_candidate(run_arg_t run_arg,sched_info_t* sched_info,char* inp, int* inp_size,int* seed_idx){

    if(*seed_idx < run_arg.seed_file_num){

        *inp_size = run_arg.seed_length[*seed_idx];

        if(memcpy(inp,run_arg.seed_inp[*seed_idx],*inp_size) == NULL){
            
            perror("seed copy failed\n");
            exit(1); 

        }

        inp[*inp_size] ='\0';

        *seed_idx = *seed_idx + 1;

    }else{

        char* candidate = create_candidate(run_arg,sched_info,inp_size);

        if(memcpy(inp,candidate,*inp_size) == NULL){

            perror("seed copy failed\n");
            exit(1); 
            
        }
        
        inp[*inp_size] ='\0';

        free(candidate);
    }
}

void fuzzer_main(config_t* usr_config,run_arg_t* usr_run_arg,input_arg_t* usr_inp_arg){
    srand(time(NULL));
    fuzzer_init(usr_config,usr_run_arg,usr_inp_arg);
    coverage_set_t cover_set;
    sched_info_t sched_info;

    if(test_config.coverage == 1){
        coverage_init(&test_config,run_arg,&cover_set);
    }
    
    struct itimerval t;
    time_t start,end;
    signal(SIGALRM,timeout_handler);

    //####
    float* total_coverage[NUM_OF_MAX];
    float* total_br_coverage[NUM_OF_MAX];

    for(int i = 0; i < run_arg.src_file_num ; i++){
        total_coverage[i] = (float*)malloc(sizeof(float)*test_config.trial);
        total_br_coverage[i] = (float*)malloc(sizeof(float)*test_config.trial);
    }
    //###

    start = clock();

    int seed_idx = 0;
    int length = 0;
    char random_inp[BUFFER_SIZE];

    for(int i = 0; i < test_config.trial ;i++){

        //#2 fuzzer_create_input();
        sched_info.list_length = run_arg.seed_file_num;

        memset(random_inp,0,sizeof(char)*BUFFER_SIZE);

        int random_size = 0;

        sellect_candidate(run_arg,&sched_info,random_inp,&random_size,&seed_idx);

        if(random_size < 0){
            perror("random input create failed\n");
            exit(1);
        }
        
        if(run_arg.fuzz_type == 1){
            
            run_arg.cmd_args[run_arg.args_num+1] = (char*)malloc(sizeof(char)*(random_size + 1));
            
            if(memcpy(run_arg.cmd_args[run_arg.args_num+1],random_inp,random_size) == NULL){
                
                perror("argument input memcpy failed\n");
                exit(1);
            
            }
            run_arg.cmd_args[run_arg.args_num+1][random_size] = '\0';
        }else{
            
        }

        //#3 fuzzer_run();

        int return_code = run(run_arg,random_inp,random_size,i);

        //#4 fuzzer_oracle;
        test_config.oracle(tmp_dir_name,i,return_code);

        if(test_config.coverage == 1){

            for(int j = 0; j < run_arg.src_file_num; j++){

                coverage_gcov(run_arg.src_file[j],run_arg.src_dir);

                read_gcov(&cover_set,&run_arg,random_inp,random_size,j,i);

                reset_gcda(run_arg,j);

                total_coverage[j][i] = (float)cover_set.total_excute_line[j];
                total_br_coverage[j][i] = (float)cover_set.total_excute_branch[j];
            }
        }

        if(return_code == 0){
            passed++;
        }else if(return_code > 0){
            failed++;
        }else{
            unresol++;
        }
    }
    end = clock();
    double excution = (float)(end-start)/CLOCKS_PER_SEC;

    printf("===================result summary=================\n");
    printf("excution time: %f\n",excution);
    printf("running program per sec: %f\n",test_config.trial/excution);
    printf("failed percentage: %f%%\n",(failed/(float)test_config.trial)*100);
    printf("trial: %d\nPassed: %d\nFailed: %d\n",test_config.trial,passed,failed);
    printf("==================================================\n");
    if(test_config.coverage == 1){
        FILE * fp = fopen("result_summary.csv","ab");
        
        if(fp == NULL){
            perror("result file failed\n");
            exit(1);
        }
        
        fprintf(fp,"Trial,line coverage , branch coverage\n");
        for(int i = 0; i < test_config.trial; i++){

            for(int j = 0; j < run_arg.src_file_num; j++){

                fprintf(fp,"%d,%0.1f,%0.1f",i+1,total_coverage[j][i],total_br_coverage[j][i]);
                if(j + 1 < run_arg.src_file_num){
                    fprintf(fp,",");
                }
            }
            fprintf(fp,"\n");
        }
        float total_line;
        float total_branch;
        // printf("accumulate: \n");
        for(int j = 0; j <run_arg.src_file_num; j++){  

            fprintf(fp,"total,%0.3f%%,%0.3f%%\n",(float)(cover_set.total_excute_line[j]*100)/(float)cover_set.code_size[j][0],(float)(cover_set.total_excute_branch[j]*100)/(float)cover_set.code_size[j][1]);
            
        }
        fclose(fp);
        // printf("==================================================\n");
        int seed_dir_length = strlen(run_arg.seed_dir);

        for(int j = 0; j < cover_set.branch_set.branch_length; j++){

            char* n_seed = (char*)malloc(sizeof(char) *(seed_dir_length + 11));

            sprintf(n_seed,"%s/%d_new_seed",run_arg.seed_dir,j);
            // printf("{ %s , %d }\n",cover_set.branch_set.input[j],cover_set.branch_set.line_num[j]);
            
            int seed_dir = open(n_seed,O_RDWR | O_TRUNC |O_CREAT ,0600);
            int sent = 0;

            while(sent < cover_set.branch_set.input_length[j]){
                sent += write(seed_dir,cover_set.branch_set.input[j]+sent,cover_set.branch_set.input_length[j]-sent);
                if(sent == -1){
                    perror("add seed failed\n");
                }
            }

            if(close(seed_dir) == -1){
                perror("close seed file failed\n");
                exit(1);
            }

            free(n_seed);

        }


        for(int i = 0; i < run_arg.src_file_num; i++){
            free(total_br_coverage[i]);
            free(total_coverage[i]);
        }

        coverage_free(&cover_set,run_arg.src_file_num);

    }

    config_free(&run_arg,&inp_arg);
    // delete_result();
}