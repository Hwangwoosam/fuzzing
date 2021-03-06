#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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

static config_t test_config; 
static pid_t child;

static int (* oracle) (char * dir_name,int trial,int ret_code) ;

static char dir_name[20];
static int failed,backslash,no_8,no_d;
static int passed;
static int unresol;
static int no_err;
static int coverage_flag;

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
    strcpy(dir_name, temp_dir) ;
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
    char buf[1024];
    int fd = open(filename,O_RDWR | O_TRUNC | O_CREAT , 0600);
    
    if(fd < 0){
        perror("output fd open failed\n");
        exit(1);
    }
    int s;
    while((s=read(pipe,buf,1024)) > 0){
        int sent = 0;
        while(sent < s){
            sent += write(fd,buf+sent,s-sent);
        }
    }

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

    char buf[1024];
    int s;

    char out_fpath[64];
    char err_fpath[64];

    sprintf(out_fpath,"%s/%d_output",dir_name,order);
    sprintf(err_fpath,"%s/%d_error",dir_name,order);

    save_data(out_fpath,pipe_stdout[0]);        
    save_data(err_fpath,pipe_stderr[0]);

    close(pipe_stderr[0]);
    close(pipe_stdout[0]);
}

void execute_target(char* inp, int inp_size){
    dup2(pipe_stdin[0],STDIN_FILENO);
    dup2(pipe_stdout[1],STDOUT_FILENO);
    dup2(pipe_stderr[1],STDERR_FILENO);
    int sent = 0;
    while(sent < inp_size){
        sent += write(pipe_stdin[1],inp + sent,inp_size - sent);
    }
    close(pipe_stdin[0]);
    close(pipe_stdin[1]);
    close(pipe_stdout[0]);
    close(pipe_stderr[0]);

    if(test_config.run_arg.fuzz_type == 1){
        test_config.run_arg.cmd_args[test_config.run_arg.args_num+1] = inp;
    }
    execv(test_config.run_arg.binary_path,test_config.run_arg.cmd_args);
    perror("execv error\n");
    exit(1);
}

int run(run_arg_t run_config ,char* random_inp,int inp_size,int order){

    char inp_fpath[64];
    sprintf(inp_fpath,"%s/%d_input",dir_name,order);
    int fd = open(inp_fpath,O_RDWR | O_TRUNC | O_CREAT , 0600);
    if(fd < 0){
        perror("input fd open failed\n");
        exit(1);
    }
    int sent = 0;
    while(sent < inp_size){
        sent += write(fd,random_inp+sent,inp_size-sent);
        if(sent == -1){
            perror("input write error\n");
            exit(1);
        }
    }
    if(close(fd) == -1 ){
        perror("input fd close failed\n");
        exit(1);
    }

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
        close(pipe_stdout[1]);
        close(pipe_stderr[1]);
        close(pipe_stdin[1]);
        close(pipe_stdin[0]);
        wait(&status);
        write_result(random_inp,inp_size,order);

    }else if(child == 0){
        alarm(run_config.timeout);
        execute_target(random_inp,inp_size);
    }else{
        perror("fork failed\n");
        exit(1);
    }
    return status;
}

void seed_search(run_arg_t* run_config){
    DIR * dp;
    struct dirent * ep;
    dp = opendir(run_config->seed_dir);
    int idx = 0;

    while(ep = readdir(dp)){
        if(ep->d_type == DT_REG){
            int length = strlen(ep->d_name);
            run_config->seed_file_name[idx] = (char*)malloc(sizeof(char)*(length + 1));
            strcpy(run_config->seed_file_name[idx],ep->d_name);
            run_config->seed_file_name[idx][length] = '\0';
            idx++;
        }
    }
    run_config->seed_file_num = idx;
}

void fuzzer_init(config_t* config){

    if(config->run_arg.src_dir == NULL){
        perror("there is no src directory\n");
        exit(1);
    }else{
        int dir_length = strlen(config->run_arg.src_dir);
        strcpy(test_config.run_arg.src_dir,config->run_arg.src_dir);

    }

    if(config->run_arg.src_file_num > 0){
        test_config.run_arg.src_file_num = config->run_arg.src_file_num; 
        for(int i = 0; i < test_config.run_arg.src_file_num; i++){

            int length = strlen(config->run_arg.src_file[i]);

            test_config.run_arg.src_file[i] = (char*)malloc(sizeof(char)*(length + 1));

            strcpy(test_config.run_arg.src_file[i],config->run_arg.src_file[i]);
            test_config.run_arg.src_file[i][length] = '\0';

            int src_dir_length = strlen(test_config.run_arg.src_dir);
            int src_file_length = strlen(test_config.run_arg.src_file[i]);

            char* file_checker = (char*)malloc(sizeof(char)* (src_dir_length + src_file_length + 2 ));

            sprintf(file_checker,"%s/%s",test_config.run_arg.src_dir,test_config.run_arg.src_file[i]);
            
            int src_exist = access(file_checker,F_OK);
            assert(src_exist == 0 && "this file doesn't exist or can't read\n");
        }
    }
    
    if(config->run_arg.binary_path != NULL){
        if(strlen(config->run_arg.binary_path) > PATH_MAX){
            perror("binary path length is too long\n");
            exit(1);
        }else{
            test_config.run_arg.binary_path = (char*)malloc(sizeof(char)*(strlen(config->run_arg.binary_path)+1));
            strcpy(test_config.run_arg.binary_path,config->run_arg.binary_path);
            int binary_exist = access(test_config.run_arg.binary_path,X_OK);
            assert(binary_exist == 0 && "this file doesn't exist or can't excute\n");
        }
    }

    if(config->run_arg.seed_dir != NULL){
        if(strlen(config->run_arg.seed_dir) > PATH_MAX){
            perror("src path length is too long\n");
            exit(1);
        }else{
            strcpy(test_config.run_arg.seed_dir,config->run_arg.seed_dir);
            int seed_exist = access(test_config.run_arg.seed_dir,00);
            assert(seed_exist == 0 && "this dirtory doesn't exist or can't excute\n");
            seed_search(&test_config.run_arg);
        }
    }

    test_config.inp_arg.f_min_len = config->inp_arg.f_min_len;
    test_config.inp_arg.f_max_len = config->inp_arg.f_max_len;
    test_config.inp_arg.f_char_start = config->inp_arg.f_char_start;
    test_config.inp_arg.f_char_range = config->inp_arg.f_char_range;
    
    test_config.run_arg.fuzz_type = config->run_arg.fuzz_type;
    test_config.run_arg.args_num = config->run_arg.args_num;

    if(test_config.run_arg.fuzz_type == 1){
        test_config.run_arg.cmd_args = (char**)malloc(sizeof(char*)*(test_config.run_arg.args_num + 3));
        test_config.run_arg.cmd_args[test_config.run_arg.args_num+2] = NULL; 
    }else{
        test_config.run_arg.cmd_args = (char**)malloc(sizeof(char*)*(test_config.run_arg.args_num + 2));
        test_config.run_arg.cmd_args[test_config.run_arg.args_num+1] = NULL;
    }
    test_config.run_arg.cmd_args[0] = (char*)malloc(sizeof(char)*(strlen(test_config.run_arg.binary_path)+1));
    strcpy(test_config.run_arg.cmd_args[0],test_config.run_arg.binary_path);
   
    if(test_config.run_arg.args_num > 0){
        for(int i = 1 ; i < test_config.run_arg.args_num + 1; i++){
            test_config.run_arg.cmd_args[i] = (char*)malloc(sizeof(char)*(strlen(config->run_arg.cmd_args[i-1] + 1)));
            strcpy(test_config.run_arg.cmd_args[i],config->run_arg.cmd_args[i-1]);
        }
    }else if(test_config.run_arg.args_num < 0){
        perror("Wrong arg size\n");
        exit(1);
    }

    test_config.run_arg.timeout = config->run_arg.timeout;
    test_config.coverage = config->coverage;
    test_config.trial = config->trial;
    test_config.oracle = config->oracle;
    
    if(test_config.inp_arg.f_min_len < 0){
        perror("Wrong min length\n");
        exit(1);
    }

    if(test_config.inp_arg.f_max_len < 0){
        perror("Wrong max length\n");
        exit(1);
    }

    if(test_config.inp_arg.f_min_len > test_config.inp_arg.f_max_len){
        perror("min length is bigger than max length\n");
        exit(1);
    }

    if(test_config.inp_arg.f_char_start + test_config.inp_arg.f_char_range > 255 ){
        perror("out of character range\n");
        exit(1);
    }

    if(test_config.inp_arg.f_char_start < 0 || test_config.inp_arg.f_char_range < 0){
        perror("char_start or char_range is not negative\n");
        exit(1);
    }

    //#3 make dir to store output
    create_dir();
}

void print_result(char* dir_name,int num,int return_code){
    char out_path[64];
    char err_path[64];
    char inp_path[64];
    sprintf(inp_path,"%s/%d_input",dir_name,num);
    sprintf(err_path,"%s/%d_error",dir_name,num);
    sprintf(out_path,"%s/%d_output",dir_name,num);

    int out_fd = open(out_path,O_RDONLY,0600);
    int err_fd = open(err_path,O_RDONLY,0600);
    int inp_fd = open(inp_path,O_RDONLY,0600);

    if(out_fd < 0){
        perror("print result output fd open failed\n");
        exit(1);
    }

    if(err_fd < 0){
        perror("print result error fd open failed\n");
        exit(1);
    }
    
    if(inp_fd < 0){
        perror("print result input fd open failed\n");
        exit(1);
    }
    int out_s,err_s,inp_s;

    char inp_buf[1024]={0,};
    char out_buf[1024]={0,};
    char err_buf[1024]={0,};
    if((out_s = read(out_fd,out_buf,1023)) < 0){
        perror("read output error\n");
    }
    if((err_s = read(err_fd,err_buf,1023)) < 0){
        perror("read error error\n");
    }

    if((inp_s = read(inp_fd,inp_buf,1023)) < 0){
        perror("read input error\n");
    }
    
    if(close(out_fd) == -1 ){
        perror("print result output fd close failed\n");
        exit(1);
    }

    if(close(err_fd) == -1 ){
        perror("print result error fd close failed\n");
        exit(1);
    }

    if(close(inp_fd) == -1){
        perror("print result input fd close failed\n");
        exit(1);
    }
    inp_buf[inp_s] ='\0';
    out_buf[out_s] ='\0';
    err_buf[err_s] ='\0';

    if(return_code == 0){
        passed++;
        if(err_s == 0){
            no_err++;
        }
        // printf("input: %s\noutput: %s\nerror: %s\nreturn code: \"PASS\"\n\n",inp_buf,out_buf,err_buf);
    }else if(return_code > 0){
        failed++;
        // printf("input: %s\noutput: %s\nerror: %s\nreturn code: \"Failed\"\n\n",inp_buf,out_buf,err_buf);
    }else{
        unresol++;
        // printf("input: %s\noutput: %s\nerror: %s\nreturn code: \"UNRESOLVED\"\n\n",inp_buf,out_buf,err_buf);
    }
}

void delete_result(){
    DIR * dp;
    struct dirent * ep;
    dp = opendir(dir_name);
    while(ep = readdir(dp)){
        char temp[256];
        strcpy(temp,dir_name);
        strcat(temp,"/");
        if(ep->d_type == DT_REG){
            strcat(temp,ep->d_name);
            remove(temp);
        }
    }
    if(rmdir(dir_name) == -1){
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
        free(run->seed_file_name[i]);
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

char* seed_read(run_arg_t* run_config,int num,int* length){
    if(num > run_config->seed_file_num){
        perror("Wrong seed file number\n");
    }

    int seed_dir_length = strlen(run_config->seed_dir);
    int seed_file_length = strlen(run_config->seed_file_name[num]);

    char* path = (char*)malloc(sizeof(char) * (seed_dir_length + seed_file_length + 2 ));

    sprintf(path,"%s/%s",run_config->seed_dir,run_config->seed_file_name[num]);

    char buf[BUFFER_SIZE];
    
    FILE* fp = fopen(path,"rb");
    int total_len = 0;
    int sent = 0;
    char* seed_input = (char*)malloc(sizeof(char)* BUFFER_SIZE);

    while((sent = fread(buf,sizeof(char),BUFFER_SIZE,fp)) > 0){
        if(total_len + sent > BUFFER_SIZE){
           char* check = realloc(seed_input,sizeof(char)*((total_len/BUFFER_SIZE)+1));
            if(check == NULL){
                perror("failed to realloc\n");
                exit(0);
            }        
        }
        if(memcpy(seed_input + total_len,buf,sent) == NULL){
            perror("seed_read memcpy failed\n");
            return NULL;
        }
        if(memset(buf,0,BUFFER_SIZE) == NULL){
            perror("buf memset failed\n");
        }
        total_len += sent;
    }
    if(total_len == 0){
        perror("No seed_input\n");
        return NULL;
    }
    *length = total_len;
    seed_input[total_len] ='\0';

    free(path);
    return seed_input;
}

void fuzzer_main(config_t* config){
    srand(time(NULL));
    fuzzer_init(config);

    coverage_set_t cover_set;

    if(test_config.coverage == 1){
        coverage_init(&test_config,&cover_set);
    }
    
    struct itimerval t;
    time_t start,end;
    signal(SIGALRM,timeout_handler);

    //####
    float* total_coverage[NUM_OF_MAX];
    float* total_br_coverage[NUM_OF_MAX];
    for(int i = 0; i < test_config.run_arg.src_file_num ; i++){
        total_coverage[i] = (float*)malloc(sizeof(float)*test_config.trial);
        total_br_coverage[i] = (float*)malloc(sizeof(float)*test_config.trial);
    }
    //###

    start = clock();
    char* seed_inp;
    int seed_idx = 0;
    int length = 0;
    char random_inp[BUFFER_SIZE];
    for(int i = 0; i < test_config.trial ;i++){
        //#2 fuzzer_create_input();
        if(i%(test_config.trial/test_config.run_arg.seed_file_num) == 0){
            if(seed_idx < test_config.run_arg.seed_file_num){
                seed_inp = seed_read(&test_config.run_arg,seed_idx,&length);
                seed_idx++;
            }
        }

        memset(random_inp,0,sizeof(char)*BUFFER_SIZE);

        int random_size = mutate(seed_inp,random_inp,length);

        if(random_size < 0){
            perror("random input create failed\n");
            exit(1);
        }
        if(test_config.run_arg.fuzz_type == 1){
            test_config.run_arg.cmd_args[test_config.run_arg.args_num+1] = random_inp;
        }

        //#3 fuzzer_run();
        printf("run\n");
        int return_code = run(test_config.run_arg,random_inp,random_size,i);
        //#4 fuzzer_oracle;
        test_config.oracle(dir_name,i,return_code);

        if(test_config.coverage == 1){
            for(int j = 0; j < test_config.run_arg.src_file_num; j++){

                coverage_gcov(test_config.run_arg.src_file[j],test_config.run_arg.src_dir);
                read_gcov(&cover_set,&test_config,random_inp,random_size,j,i);
                // reset_gcda(test_config,j);
                int exe_accumulate = 0;
                int bran_accumulate = 0;
                
                for(int k = 0; k <cover_set.code_size[j][1]; k++){
                    if(cover_set.line_check[j][k] != 0){
                        exe_accumulate++;
                    }
                }

                for(int k = 0; k <cover_set.code_size[j][2]; k++){
                    if(cover_set.branch_check[j][k] != 0){
                        bran_accumulate++;
                    }
                }

                total_coverage[j][i] = ((float)exe_accumulate/(float)cover_set.code_size[j][1])*100;
                total_br_coverage[j][i] = ((float)bran_accumulate/(float)cover_set.code_size[j][2])*100;

            }
        }

        print_result(dir_name,i,return_code);
    }
    end = clock();
    double excution = (float)(end-start)/CLOCKS_PER_SEC;
    printf("===================result summary=================\n");
    printf("excution time: %f\n",excution);
    printf("running program per sec: %f\n",test_config.trial/excution);
    printf("failed percentage: %f%%\n",(failed/(float)test_config.trial)*100);
    printf("trial: %d\nPassed: %d\nFailed: %d\nPerfect: %d\n",test_config.trial,passed,failed,no_err);
    printf("==================================================\n");
    if(test_config.coverage == 1){
        printf("===================coverage=======================\n");
        for(int i = 0; i < test_config.trial; i++){
             printf("Trial[%d] coverage :\n",i+1);
            for(int j = 0; j < test_config.run_arg.src_file_num; j++){
                printf("%0.3f%%  %0.3f%%\n",total_coverage[j][i],total_br_coverage[j][i]);
            }
            printf("\n");
        }
        float total_line;
        float total_branch;
        printf("accumulate: \n");
        for(int j = 0; j <cover_set.src_file_num; j++){
            total_line = 0;
            total_branch = 0;
            for(int k = 0; k < cover_set.code_size[j][1]; k++){
                if(cover_set.line_check[j][k] == 1){
                    total_line++;
                }
            }
            for(int k = 0; k < cover_set.code_size[j][2]; k++){
                if(cover_set.branch_check[j][k] == 1){
                    total_branch++;
                }
            }
            printf("%0.3f%%  %0.3f%%\n",(total_line*100)/cover_set.code_size[j][1],(total_branch*100)/cover_set.code_size[j][2]);
        }
        printf("==================================================\n");
        
        int seed_dir_length = strlen(test_config.run_arg.seed_dir);

        for(int j = 0; j < cover_set.branch_set.branch_length; j++){

            char* n_seed = (char*)malloc(sizeof(char) *(seed_dir_length + 12));

            sprintf(n_seed,"%s/%d_new_seed",test_config.run_arg.seed_dir,j);
            printf("{ %s , %d }\n",cover_set.branch_set.input[j],cover_set.branch_set.line_num[j]);
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

        for(int i = 0; i < test_config.run_arg.src_file_num; i++){
            free(total_br_coverage[i]);
            free(total_coverage[i]);
        }

        coverage_free(&cover_set,test_config.run_arg.src_file_num);
    }

    config_free(&test_config.run_arg,&test_config.inp_arg);
    // delete_result();
}