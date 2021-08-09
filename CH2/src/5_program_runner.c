#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define PASS "PASS"
#define FAIL "FAIL"
#define UNRESOLVED "UNRESOLVED"

int pipes[2];
int pipes2[2];
int pipes3[2];

typedef struct result{
    char* output ;
    char* error;
    char* outcome;
    int return_code;
}result_t;

result_t* subprocess(char* program,char* str){
    result_t* temp = (result_t*)malloc(sizeof(result_t));

    if(pipe(pipes) != 0) {
        perror("pipe") ;
        exit(1) ;
    }
    if (pipe(pipes2) != 0) {
        perror("pipe2") ;
        exit(1) ;
    }
     if (pipe(pipes3) != 0) {
        perror("pipe3") ;
        exit(1) ;
    }

    pid_t child = fork();

    if(child > 0){
        close(pipes2[1]);
        close(pipes3[1]);

        // dprintf(pipes[1],"%s",str);
        write(pipes[1],str,sizeof(str));
        close(pipes[1]);

        int status;
        wait(&status);
        temp->return_code = status;

        if(temp->return_code == 0){
            temp->outcome = (char*)malloc(sizeof(char)*strlen(PASS)+1);
            strcpy(temp->outcome,PASS);
        }else if(temp->return_code < 0){
            temp->outcome = (char*)malloc(sizeof(char)*strlen(FAIL)+1);
            strcpy(temp->outcome,FAIL);
        }else{
            temp->outcome = (char*)malloc(sizeof(char)*strlen(UNRESOLVED)+1);
            strcpy(temp->outcome,UNRESOLVED);
        }

        temp->output = (char*)malloc(sizeof(char)*1024);
        if(read(pipes2[0],temp->output,1024) < 0){
            printf("read failed\n");
            exit(0);
        }

        temp->error = (char*)malloc(sizeof(char)*1024);
        if(read(pipes3[0],temp->error,1024) < 0){
            printf("read failed\n");
            exit(0);
        }
    }else if(child == 0){
        dup2(pipes[0],STDIN_FILENO);
        dup2(pipes2[1],STDOUT_FILENO);
        dup2(pipes3[1],STDERR_FILENO);

        close(pipes[0]);
        close(pipes[1]);
        close(pipes2[0]);
        close(pipes3[0]);

        execlp(program,program,NULL);
    }
    return temp;
}

void delete_result(result_t* tmp){
    free(tmp->error);
    free(tmp->outcome);
    free(tmp->output);
    free(tmp);
}