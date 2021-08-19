#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int coverage_compile(char *test_file,char* exc_file);
int coverage_excute(char* exc_name);
int coverage_gcov(char* test_file);
char* get_filename(char* path);
void make_gcov(int argc,char* argv[]);
void read_gcov(char* path);