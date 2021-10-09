#include <string.h>
#include <regex.h>

typedef struct url{
    char* scheme;
    char* netloc;
    char* path;
    char* query;
    char* fragment;
}url_t;

int url_parse(char* str,url_t* url);

void free_url(url_t* url);

int http_program(char * str,url_t* url);

int is_valid_url(char* str,int size,url_t* result);

int delete_random(char* str,char* target,int str_size);

int insert_random(char* str,char* target,int str_size);

int flip_random(char* str,char* target,int str_size);

int mutate(char* str,char* target,int str_size);