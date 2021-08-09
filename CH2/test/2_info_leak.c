#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "../include/0_fuzzer.h"
#include "../include/2_info.h"

char * secret;
void test(char* str){
    char* result = heartbeat(str,strlen(str),secret);
    printf("result: %s\n",result);
    free(result);
}

void test2(){
    for(int i = 0; i < 10; i++){
        char *s = heartbeat(fuzzer(100,32,32),(rand()%500)+1,secret);
        char uninitialized_memory_marker[] = "deadbeef";
        printf("%s\n",s);
        assert(strstr(s,uninitialized_memory_marker) == NULL);
        assert(strstr(s,"secret") == NULL);
        free(s);
    }
}
int main(){
    srand(time(NULL));
    secret = make_secret();

    test("potato");
    test("bird");
    test("hat");
    test2();

    free(secret);
}