#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PASS "PASS"
#define FAIL "FAIL"
#define UNRESOLVED "UNRESOLVED"

typedef struct Runner{
    char * pass;
    char * fail;
    char * unresolved;
}runner_t;

runner_t* init();

char** run(runner_t* runner, char*str);