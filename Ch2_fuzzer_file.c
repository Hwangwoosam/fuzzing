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

char * fuzzer(int max_length,int char_start,int char_range,int * length){
    int string_length = rand()%(max_length+1);
    char* out = (char*)malloc(sizeof(char)*string_length);

    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(char_range) + char_start;
        out[i] = tmp;
    }
    *length = string_length;

    return out;
}

file_info_t write_f(char* name,char* data,int length){
    char* file;
    strcpy(file,dir_name);
    strcat(file,"/");
    strcat(file,name);

    FILE* fp = fopen(file,"w");
    if(fp == NULL){
        printf("failed\n");
    }

    file_info_t temp;
    temp.data = (char*)malloc(sizeof(char)*length);
    strcpy(temp.data,data);
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
    printf("flag\n");
    if(stdout_flag == 1){
        char temp[128];
        strcpy(temp,dir_name);
        strcat(temp,"/std_out.txt");
        temp_file_make(temp);
        std_out = open(temp,O_RDWR|O_CREAT);
    }

    if(stderr_flag == 1){
        char temp2[128];
        strcpy(temp2,dir_name);
        strcat(temp2,"/std_err.txt");
        temp_file_make(temp2);
        std_err = open(temp2,O_RDWR|O_CREAT);
    }
    printf("redirection\n");
    if(stderr_flag == 1){
            dup2(std_err,STDERR_FILENO); 
    }
    if(stdout_flag == 1){
            dup2(std_out,STDOUT_FILENO);
    }
    printf("execl help\n");
    int return_code;
    pid_t child = fork();
    if(child > 0){
        wait(&return_code);
        // temp_file_close();
    }else if(child == 0){
        char temp3[128] = "/bin/";
        strcat(temp3,argv[0]);
        execl(temp3,temp3,"-q",argv[1],(char*)0);
    }
    printf("end\n");
    wait(0x0);
}



int main(){
    //Test
    srand(time(NULL));
    for(int i = 0; i < 100;i++){
        // printf("%d :i\n",i);
        int length;
        char* data = fuzzer(100,32,32,&length);

        FILE* fp = fopen("test.txt","w+");
        data = (char*)realloc(data,(length+4)*sizeof(char));
        
        char quit[4] ={"quit"};
        strcat(data,quit);
        fputs(data,fp);
        fclose(fp);
        
        char* test[] = {"bc","test.txt"};
        subprocess_run(test,sizeof(test)/sizeof(char*),0,1,1);
    }
    // fuzzing_file();
}