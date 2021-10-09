#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../../Fuzzer/include/config.h"

typedef struct coverage_set{
    int* code_size;
    char* src_file_name;
    int* line_check;     
    int* execute_check;
    int* branch_check;
    int* e_branch_check;
}coverage_set_t;

void coverage_init(config_t test_config,coverage_set_t* cover_set,int trial);
int coverage_compile(char *test_file,char* exc_file);
int coverage_excute(char* exc_name);
int coverage_gcov(char* test_file);
char* get_filename(char* path);
void make_gcov(int argc,char* argv[]);
void read_gcov(char* path,int* line_result,int* branch_result,int* list,int* total_branch);
int* get_code_line(char* path);
void reset_gcda(char* path);
void coverage_free(coverage_set_t* cover_set);