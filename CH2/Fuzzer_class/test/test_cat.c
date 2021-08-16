#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "../include/fuzzer.h"
int oracle(char* dir_name,int trial,int return_code){

    char inp_fpath[64];
    char out_fpath[64];
    sprintf(inp_fpath,"%s/%d_input",dir_name,trial);
    sprintf(out_fpath,"%s/%d_output",dir_name,trial);
    
    if(return_code == 0){
        int inp_fd = open(inp_fpath,O_RDONLY);
        int out_fd = open(out_fpath,O_RDONLY);
    
        if(inp_fd < 0){
            perror("oracle: input fd open failed\n");
            exit(1);
        }

        if(out_fd < 0){
            perror("oracle: output fd open failed\n");
            exit(1);
        }
        while(1){
            char inp_buf[1024];
            char out_buf[1024];
            int inp_s,out_s;
            inp_s = read(inp_fd,inp_buf,1023);
            inp_buf[inp_s] = '\0';
            out_s = read(out_fd,out_buf,1023);
            out_buf[out_s] = '\0';
            assert(inp_s == out_s);
            assert(strcmp(inp_buf,out_buf) == 0);
            if(inp_s == 0 && out_s == 0){
                break;
            }
        }
        if(close(inp_fd) == -1 ){
            perror("oracle: input fd close failed\n");
            exit(1);
        }

        if(close(out_fd) == -1 ){
            perror("oracle: output fd close failed\n");
            exit(1);
        }
    }
    return 0;
}
void set_configs(test_config_t * config){
    strcpy(config->run_arg.binary_path,"/bin/cat");
    char* test_arg[] = {"/bin/cat",NULL};
    config->run_arg.cmd_args = test_arg;
    config->run_arg.args_num = 0;
    config->oracle = oracle;

}

int main(){
    test_config_t temp ;
    init_config(&temp);
    set_configs(&temp);
    fuzzer_main(&temp);
}