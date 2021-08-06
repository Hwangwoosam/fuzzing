#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "../include/fuzzer.h"

char template[] = "tmp.XXXXXX";
char *dir_name;


int pipes[2]; //stdin 0 read 1 write
int pipes2[2];//stderr

void temp_file_close(){
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

void subprocess_run(char* program,char* data,int num,int data_size,int* err_num,int* non_err_num){
    int std_out,std_err;
    if(dir_name == 0x0){
        dir_name = mkdtemp(template);
    }
    char out_temp[128];
    char err_temp[128];
    char input_temp[128];
    
    sprintf(err_temp,"%s/%d_error.txt",dir_name,num);
    sprintf(out_temp,"%s/%d_output.txt",dir_name,num);
    sprintf(input_temp,"%s/%d_input.txt",dir_name,num);

    FILE * tmp = fopen(input_temp,"w+");
    if(tmp == 0x0){
        perror("input file open failed\n");
    }
    fwrite(data,1,data_size,tmp);
    fclose(tmp);

    if (pipe(pipes) != 0) {
        perror("pipe") ;
        exit(1) ;
    }
     if (pipe(pipes2) != 0) {
        perror("pipe2") ;
        exit(1) ;
    }

    int return_code;
    int Devnull = open("/dev/null",O_RDONLY);

    int flag = 0;
    int non_flag = 0;

    pid_t child = fork();
    if(child == 0){
  
        close(pipes[0]);
        close(pipes2[0]);

        dup2(Devnull,0);
        dup2(pipes[1],1);
        dup2(pipes2[1],2);

        execlp(program,program,"-q",input_temp,(char*)0);

    }else if(child > 0){
        
        wait(&return_code);
        close(pipes[1]);
        close(pipes2[1]);

        char buf[1024];
        int s;

        FILE * out = fopen(out_temp,"w+");
        if(out == 0x0){
            perror("input file open failed\n");
        }
        while((s=read(pipes[0],buf,1024))>0){
            fwrite(buf,1,s,out);
            // printf("%d: num\n",num);
            non_flag++;
        }
        fclose(out);

        FILE * err = fopen(err_temp,"w+");
        if(err == 0x0){
            perror("input file open failed\n");
        }
        while((s=read(pipes2[0],buf,1024))>0){
            fwrite(buf,1,s,err);
            flag++;
        }
        fclose(err);
    }
    if(non_flag){
        int num = *non_err_num;
        num ++;
        *non_err_num = num;
    }
    if(flag){
        int num2 = *err_num;
        num2 ++;
        *err_num = num2;
    }
    // printf("%d return code\n",return_code);
}

int main(){
    //Test
    srand(time(NULL));
    char* test = "bc";
    int err_num = 0;
    int non_err_num =0;
    for(int i = 0; i < 100;i++){
        char* data = fuzzer(100,32,32);
        subprocess_run(test,data,i,strlen(data),&err_num,&non_err_num);
        free(data);
    }
    printf("%d(err) : %d(non_err)\n",err_num,non_err_num);
    temp_file_close(dir_name);
}