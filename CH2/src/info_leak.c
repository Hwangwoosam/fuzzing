#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/fuzzer.h"

char * heartbeat(char* reply,int length,char* memory){
    for(int i = 0; i < strlen(reply); i++){
        memory[i] = reply[i];
    }
    char * s = (char*)malloc(sizeof(char)*length);
    for(int i = 0; i <length; i++){
        s[i] = memory[i];
    }

    return s;
}

char * make_secret(){
    char* secrets = (char*)malloc(sizeof(char)*2048);
    
    sprintf(secrets,"<space for reply>%s<secret-certificate>%s<secret-key>%s<other-secrets>",fuzzer(100,32,32),fuzzer(100,32,32),fuzzer(100,32,32));
    
    char uninitialized_memory_marker[] = "deadbeef";
    int un_size = strlen(uninitialized_memory_marker);

    while (strlen(secrets) + un_size +1 < 2048)
    {
        strcat(secrets,uninitialized_memory_marker);
    }
    return secrets;
}