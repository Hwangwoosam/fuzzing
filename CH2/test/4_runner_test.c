#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/4_runner.h"

void test(char* str){
    runner_t* tmp = init();
    char** test = run(tmp,str);
    printf("%s %s\n",test[0],test[1]);
    free(tmp);
    free(test);
}

int main(){
    test("some input");
    test("\0");
    test("    ");
}