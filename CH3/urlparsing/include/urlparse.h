#include <string.h>
#include <regex.h>

typedef struct url{
    char* scheme;
    char* netloc;
    char* path;
    char* query;
    char* fragment;
}url_t;

url_t* url_parse(char* str);

void free_url(url_t* url);

int http_program(char * str);

int is_valid_url(char* str);