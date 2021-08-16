#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>

#include "../include/create_inp.h"
#include "../include/fuzzer.h"

static input_arg_t input_config;
static run_arg_t run_config;
static int trial;
static pid_t child;
static int (* oracle) (char * dir_name,int trial,int ret_code) ;

static char dir_name[20];

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

int run(run_arg_t run_config ,char* random_inp,int inp_size,int order){
    
    int pipe_stdin[2];
    int pipe_stdout[2];
    int pipe_stderr[2];

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
        
        write(pipe_stdin[1],random_inp,inp_size);

        close(pipe_stdout[1]);
        close(pipe_stderr[1]);
        close(pipe_stdin[1]);

        wait(&status);

        char buf[1024];
        int s;

        char out_fpath[64];
        char inp_fpath[64];
        char err_fpath[64];

        sprintf(inp_fpath,"%s/%d_input",dir_name,order);
        sprintf(out_fpath,"%s/%d_output",dir_name,order);
        sprintf(err_fpath,"%s/%d_error",dir_name,order);

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

        save_data(out_fpath,pipe_stdout[0]);
        
        save_data(err_fpath,pipe_stderr[0]);
    

    }else if(child == 0){
        dup2(pipe_stdin[0],STDIN_FILENO);
        dup2(pipe_stdout[1],STDOUT_FILENO);
        dup2(pipe_stderr[1],STDERR_FILENO);

        close(pipe_stdin[0]);
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(pipe_stderr[0]);
        
        execv(run_config.binary_path,run_config.cmd_args);
        perror("execv error\n");
        exit(1);
    }else{
        perror("fork failed\n");
        exit(1);
    }

    return status;
}

void fuzzer_init(test_config_t* config){

    // //#1 path and args check
    int fp_exist = access(config->run_arg.binary_path,X_OK);
    assert(fp_exist == 0 && "this file doesn't exist or can't excute\n");
    
    //#2 invalid input
    input_config.f_min_len = config->inp_arg.f_min_len;
    input_config.f_max_len = config->inp_arg.f_max_len;
    input_config.f_char_start = config->inp_arg.f_char_start;
    input_config.f_char_range = config->inp_arg.f_char_range;

    if(strlen(run_config.binary_path) > PATH_MAX){
        perror("path length is too long\n");
        exit(1);
    }else{
        strcpy(run_config.binary_path,config->run_arg.binary_path);
    }

    run_config.args_num = config->run_arg.args_num;
    run_config.cmd_args = (char**)malloc(sizeof(char*)*run_config.args_num + 2);
    run_config.cmd_args[0] = (char*)malloc(sizeof(char)*(strlen(run_config.binary_path)+1));
    strcpy(run_config.cmd_args[0],run_config.binary_path);
    run_config.cmd_args[1] = NULL;

    if(run_config.args_num > 0){
        for(int i = 1 ; i < run_config.args_num + 1; i++){
            run_config.cmd_args[i] = (char*)malloc(sizeof(char)*(strlen(config->run_arg.cmd_args[i-1] + 1)));
            strcpy(run_config.cmd_args[i],config->run_arg.cmd_args[i-1]);
        }
        run_config.cmd_args[run_config.args_num] = "NULL";

    }else if(run_config.args_num < 0){
        perror("Wrong arg size\n");
        exit(1);
    }
    run_config.timeout = config->run_arg.timeout;

    trial = config->trial;
    oracle = config->oracle;

    //#3 make dir to store output
    create_dir();
}

void prine_result(char* dir_name,int trial){

}

void fuzzer_main(test_config_t* config){
    srand(time(NULL));

    fuzzer_init(config);

    struct itimerval t;

    signal(SIGALRM,timeout_handler);
    
    t.it_value.tv_sec = run_config.timeout;
    t.it_interval = t.it_value;

    for(int i = 0; i < trial ;i++){
        setitimer(ITIMER_REAL,&t,0x0);
        //#2 fuzzer_create_input();
        char* random_inp = (char*)malloc(sizeof(char)*(input_config.f_max_len+1));
    
        int random_size;
        create_inp(random_inp,&random_size,input_config);

        //#3 fuzzer_run();
        int return_code = run(run_config,random_inp,random_size,i);
        //#4 fuzzer_oracle;
        config->oracle(dir_name,i,return_code);

        free(random_inp);
    }
}