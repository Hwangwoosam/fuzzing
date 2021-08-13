#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <time.h>

#include "create_inp.h"
#include "fuzzer.h"

static input_arg_t input_config;
static run_arg_t run_config;
static int trial;
static int (* oracle) (char * dir_name,int trial) ;

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

void run(run_arg_t run_config ,char* random_inp,int inp_size,int order){
    
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

    pid_t child = fork();
    if(child > 0){
        write(pipe_stdin[1],random_inp,inp_size);

        close(pipe_stdout[1]);
        close(pipe_stderr[1]);
        close(pipe_stdin[1]);

        int status;
        wait(&status);

        char buf[1024];
        int s;

        char out_fpath[64];
        char inp_fpath[64];
        char err_fpath[64];

        sprintf(inp_fpath,"%s/%d_input",dir_name,order);
        sprintf(out_fpath,"%s/%d_output",dir_name,order);
        sprintf(err_fpath,"%s/%d_error",dir_name,order);

        int fd = open(inp_fpath,O_RDWR | O_TRUNC | O_CREAT , 0644);
        if(fd < 0){
            perror("input fd open failed\n");
            exit(1);
        }
        if(write(fd ,random_inp,inp_size) == -1){
            perror("write error\n");
            exit(1);
        };

        if(close(fd) == -1 ){
            perror("input fd close failed\n");
            exit(1);
        }

        fd = open(out_fpath,O_RDWR | O_TRUNC | O_CREAT , 0644);
        if(fd < 0){
            perror("output fd open failed\n");
            exit(1);
        }
        while((s=read(pipe_stdout[0],buf,1024)) > 0){
            write(fd,buf,s);
        }
        if(close(fd) == -1 ){
            perror("output fd close failed\n");
            exit(1);
        }

        fd = open(err_fpath,O_RDWR| O_TRUNC | O_CREAT,0644);
        if(fd < 0){
            perror("err fd open failed\n");
            exit(1);
        }
        write(fd,&status,sizeof(status));
        while((s=read(pipe_stderr[0],buf,1024) > 0)){
            write(fd,buf,s);
        }
        if(close(fd) == -1 ){
            perror("err fd close failed\n");
            exit(1);
        }

    }else if(child == 0){
        
        dup2(pipe_stdin[0],STDIN_FILENO);
        dup2(pipe_stdout[1],STDOUT_FILENO);
        dup2(pipe_stderr[1],STDERR_FILENO);

        close(pipe_stdin[0]);
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(pipe_stderr[0]);
        char* arg[] = {run_config.binary_path,NULL};
        execv(run_config.binary_path,arg);
    }else{
        perror("fork failed\n");
        exit(1);
    }

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

    strcpy(run_config.binary_path,config->run_arg.binary_path);
    run_config.timeout = config->run_arg.timeout;
    trial = config->trial;
    oracle = config->oracle;

    //#3 make dir to store output
    create_dir();
}

void fuzzer_main(test_config_t* config){
    srand(time(NULL));

    fuzzer_init(config);

    for(int i = 0; i < trial ;i++){

        //#2 fuzzer_create_input();
        char* random_inp = (char*)malloc(sizeof(char)*(input_config.f_max_len+1));
    
        int random_size;
        fuzzer(random_inp,&random_size,input_config);

        //#3 fuzzer_run();
        run(run_config,random_inp,random_size,i);

        //#4 fuzzer_oracle;
        config->oracle(dir_name,i);

        free(random_inp);
    }
}