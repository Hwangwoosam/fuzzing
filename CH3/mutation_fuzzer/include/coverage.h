#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include <sys/wait.h>

typedef struct coverage_set{
    int** code_size;
    char** src_file_name;
    int src_file_num;
    int** line_check;     // total line
    int** execute_check;  // excute line
    int** branch_check;
    int** e_branch_check;
    char** branch_set;
    int* branch_length;
}coverage_set_t;

void coverage_init(config_t* config,coverage_set_t* cover_set);
int coverage_excute(char* exc_name);
int coverage_gcov(char* test_file,char* src_dir);
int get_filename(char* path,char* target);
char* remove_type(char* path);
void make_gcov(int argc,char* argv[]);
void read_gcov(coverage_set_t* cover_set,config_t* config,char* random, int random_size,int num,int trial);
int* get_code_line(char* path);
void reset_gcda(config_t config,int src_num);
void coverage_free(coverage_set_t* cover_set,int num);