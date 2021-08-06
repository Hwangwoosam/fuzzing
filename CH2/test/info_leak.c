#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "../include/fuzzer.h"
#include "../include/info.h"

char * secret;
void test_potato(){
    char* result = heartbeat("potato",6,secret);
    printf("result: %s\n",result);
    free(result);
}

void test2_bird(){
    char* result2 = heartbeat("bird",4,secret);
    printf("result2: %s\n",result2);
    free(result2);
}

void test3_hat(){
    char* result3 = heartbeat("hat",500,secret);
    printf("result3: %s\n",result3);
    free(result3);
}

void test4(){
    for(int i = 0; i < 10; i++){
        char *s = heartbeat(fuzzer(100,32,32),(rand()%500)+1,secret);
        char uninitialized_memory_marker[] = "deadbeef";
        printf("%s\n",s);
        assert(strstr(s,uninitialized_memory_marker) == NULL);
        assert(strstr(s,"secret"));
        free(s);
    }
}
int main(){
    srand(time(NULL));
    secret = make_secret();
    // test_potato();
    // test2_bird();
    test3_hat();
    // test4();
    free(secret);
}