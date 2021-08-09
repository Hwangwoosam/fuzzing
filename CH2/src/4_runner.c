#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/4_runner.h"

runner_t* init(){
    runner_t* temp = (runner_t*)malloc(sizeof(runner_t));  ;
    temp->pass = (char*)malloc(sizeof(char)*strlen(PASS)+1);
    strcpy(temp->pass,PASS);
    temp->fail = (char*)malloc(sizeof(char)*strlen(FAIL)+1);
    strcpy(temp->fail,FAIL);
    temp->unresolved = (char*)malloc(sizeof(char)*strlen(UNRESOLVED)+1);
    strcpy(temp->unresolved,UNRESOLVED);
    return temp;
}

char** run(runner_t* runner, char*str){
    char** temp = (char**)malloc(sizeof(char*)*2);
    temp[0] = str;
    temp[1] = runner->unresolved;
    return temp;
}