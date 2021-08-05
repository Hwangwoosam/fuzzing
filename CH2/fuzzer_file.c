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

char template[] = "tmp.XXXXXX";
char *dir_name;


int pipes[2]; //stdin 0 read 1 write
int pipes2[2];//stderr


char * fuzzer(int max_length,int char_start,int char_range){
    int string_length = rand()%(max_length+1);
    char* out = (char*)malloc(sizeof(char)*string_length);

    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(char_range) + char_start;
        out[i] = tmp;
    }
    out[string_length-1] ='\0';
    return out;
}

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

void subprocess_run(char* program,char* data,int num,int data_size){
    int std_out,std_err;
    //input = dir_name[0], out = dir_name[1] , err = dir_name[2]
    if(dir_name == 0x0){
        dir_name = mkdtemp(template);
    }
    char out_temp[128];
    char err_temp[128];
    char input_temp[128];
    char number[64];
    
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
        while((s=read(pipes[0],buf,1023))>0){
            buf[s] = 0x0;
            fwrite(buf,1,s,out);
        }
        fclose(out);

        FILE * err = fopen(err_temp,"w+");
        if(err == 0x0){
            perror("input file open failed\n");
        }
        while((s=read(pipes2[0],buf,1023))>0){
            buf[s] = 0x0;
            fwrite(buf,1,s,err);
        }
        fclose(err);
    }
    printf("%d return code\n",return_code);
}

int main(){
    //Test
    srand(time(NULL));
    char* test = "bc";
    for(int i = 0; i < 100;i++){
        char* data = 0x0;
        data = fuzzer(100,32,32);
        subprocess_run(test,data,i,strlen(data));
        free(data);
    }
    temp_file_close(dir_name);
}