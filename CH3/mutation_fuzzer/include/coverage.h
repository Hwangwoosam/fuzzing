#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include <sys/wait.h>
typedef struct branch_set{
    char* input;
    int branch_num;
}branch_set_t;

typedef struct coverage_set{
    int* code_size;
    char* src_file_name;
    int* line_check;     
    int* execute_check;
    int* branch_check;
    int* e_branch_check;
    branch_set_t* branch_list;
    int list_length;
}coverage_set_t;

void coverage_init(run_arg_t run_config,coverage_set_t* cover_set,int trial);
int coverage_compile(char *test_file,char* exc_file);
int coverage_excute(char* exc_name);
int coverage_gcov(char* test_file);
char* get_filename(char* path);
void make_gcov(int argc,char* argv[]);
void read_gcov(char* path,int* line_result,int* branch_result,int* list,int* total_branch,branch_set_t* branch_list,int* list_length,char* random, int random_size);
int* get_code_line(char* path);
void reset_gcda(char* path);
void coverage_free(coverage_set_t* cover_set);