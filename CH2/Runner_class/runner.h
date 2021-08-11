#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASS "PASS"
#define FAIL "FAIL"
#define UNRESOLVED "UNRESOLVED"

typedef struct Runner{
    char* input;
    char* outcome;
    int inp_size;

    void (* free_inp)(struct Runner);
    int (* run) (struct Runner *,char *,int);
} Runner;

void Runner_init(Runner* runner);

int run(Runner*, char* , int);

void free_input(Runner);