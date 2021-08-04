#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

char template[] = "tmp.XXXXXX";
char *dir_name;


typedef struct file_info{
    char* data;
    int length;
}file_info_t;

file_info_t write_f(char* name){
    char* file;
    strcpy(file,dir_name);
    strcat(file,"/");
    strcat(file,name);

    FILE* fp = fopen(file,"w");
    if(fp == NULL){
        printf("failed\n");
    }

    file_info_t temp;
    temp.data = fuzzer(1000,'a',26,&temp.length);
    fwrite(temp.data,sizeof(char),temp.length,fp);
    fclose(fp);
    return temp;
}

file_info_t read_f(char* name,int length){
    FILE* fp = fopen(name,"r");
    file_info_t temp;
    temp.length = length;
    temp.data = (char*)malloc(sizeof(char)*length);
    fread(temp.data,sizeof(char),temp.length,fp);
    fclose(fp);
    return temp;
}

void temp_file_make(char* file){
    FILE * fp = fopen(file,"w+");
    if(fp == NULL){
        printf("temp make failed\n");
    }
    fclose(fp);
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

void subprocess_run(char* argv[],int argc,int stdin_flag, int stdout_flag, int stderr_flag){
    int std_out,std_err;
    dir_name = mkdtemp(template);
    if(stdout_flag == 1){
        char temp[128];
        strcpy(temp,dir_name);
        strcat(temp,"/std_out.txt");
        temp_file_make(temp);
        // std_out = open(temp,O_RDWR|O_CREAT);
        std_out = open(temp,O_CREAT|O_RDWR|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO); 
    }

    if(stderr_flag == 1){
        char temp2[128];
        strcpy(temp2,dir_name);
        strcat(temp2,"/std_err.txt");
        temp_file_make(temp2);
        // std_err = open(temp2,O_RDWR|O_CREAT);
        std_err = open(temp2,O_CREAT|O_RDWR|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO); 
    }
    // if(stderr_flag == 1){
    //         dup2(std_err,STDERR_FILENO); 
    // }
    // if(stdout_flag == 1){
    //         dup2(std_out,STDOUT_FILENO);
    // }
    int return_code;
    pid_t child = fork();
    if(child > 0){
        printf("parent\n");
        wait(&return_code);
        printf("%d return\n",return_code);
        temp_file_close();
    }else if(child == 0){
        // printf("child\n");
        // printf("%s %s\n",argv[0],argv[1]);
        // execl(argv[0],argv[1],NULL);
        
        // fputs("quit",STDIN_FILENO);
        char temp3[20] = "bc";
        execl("/bin/cat","test.txt|bc",(char*)0);
    }
    exit(0);
}



int main(){
    //Test
    
    char* test[] = {"bc","test.txt"};
    subprocess_run(test,sizeof(test)/sizeof(char*),0,1,1);
    // fuzzing_file();
}