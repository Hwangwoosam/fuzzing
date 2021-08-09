#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "../include/5_program_runner.h"

int main(){
    char program[] = "cat";
    result_t* temp = subprocess(program,"hello");
    printf("CompletedProcess:\nargs= \'%s\'\nreturncode = %d\nstdout= \'%s\'\nstderr= \'%s\'\nprogram_test= \'%s\'\n",program,temp->return_code,temp->output,temp->error,temp->outcome);
    delete_result(temp);
}