#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PASS "PASS"
#define FAIL "FAIL"
#define UNRESOLVED "UNRESOLVED"

int pipe_stdin[2];
int pipe_stdout[2];
int pipe_stderr[2];

typedef struct result{
    char* output ;
    char* error;
    char* outcome;
    int return_code;
}result_t;

result_t* subprocess(char* program,char* str){
    int length = strlen(str);
    if(pipe(pipe_stdin) != 0) {
        perror("pipe") ;
        exit(1) ;
    }
    if (pipe(pipe_stdout) != 0) {
        perror("pipe2") ;
        exit(1) ;
    }
     if (pipe(pipe_stderr) != 0) {
        perror("pipe3") ;
        exit(1) ;
    }

    pid_t child = fork();

    if(child > 0){
        result_t* temp = (result_t*)malloc(sizeof(result_t));
        write(pipe_stdin[1],str,strlen(str));

        close(pipe_stdout[1]);
        close(pipe_stderr[1]);
        close(pipe_stdin[1]);

        int status;
        wait(&status);
        temp->return_code = status;

        if(temp->return_code == 0){
            temp->outcome = (char*)malloc(sizeof(char)*(strlen(PASS)+1));
            strcpy(temp->outcome,PASS);
        }else if(temp->return_code < 0){
            temp->outcome = (char*)malloc(sizeof(char)*(strlen(FAIL)+1));
            strcpy(temp->outcome,FAIL); 
        }else{
            temp->outcome = (char*)malloc(sizeof(char)*(strlen(UNRESOLVED)+1));
            strcpy(temp->outcome,UNRESOLVED);
        }

        temp->output = (char*)malloc(sizeof(char)*1024);    //TODO append
        if(read(pipe_stdout[0],temp->output,length) < 0){
            printf("read failed\n");
            exit(0);
        }
        temp->output[length] = '\0';
        temp->error = (char*)malloc(sizeof(char)*1024);     //TODO append
        if(read(pipe_stderr[0],temp->error,1024) < 0){
            printf("read failed\n");
            exit(0);
        }
        return temp;
    }else if(child == 0){

        dup2(pipe_stdin[0],STDIN_FILENO);
        dup2(pipe_stdout[1],STDOUT_FILENO);
        dup2(pipe_stderr[1],STDERR_FILENO);

        close(pipe_stdin[0]);
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(pipe_stderr[0]);

        execlp(program,program,NULL);
    }
}

void delete_result(result_t* tmp){
    free(tmp->error);
    free(tmp->outcome);
    free(tmp->output);
    free(tmp);
} 