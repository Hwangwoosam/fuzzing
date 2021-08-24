#include <stdio.h>
#include <stdlib.h>
#include "../include/urlparse.h"


url_t* url_parse(char* str){
    url_t* result = (url_t*)malloc(sizeof(url_t));
    regex_t state;
    char url_regex[] = "^(http|https)://([^/?]*)/?([^?]*)\\?\?([^/]*)#?(.*)";
    if(regcomp(&state,url_regex,REG_EXTENDED)){
        perror("regcomp failed\n");
    }
    
    regmatch_t pmatch[6];
    int status = regexec(&state,str,6,pmatch,0);
    if(status == 0){
        result->scheme = (char*)malloc(sizeof(char)*(pmatch[1].rm_eo-pmatch[1].rm_so));
        memcpy(result->scheme,&str[pmatch[1].rm_so],pmatch[1].rm_eo-pmatch[1].rm_so);
        
        result->netloc = (char*)malloc(sizeof(char)*(pmatch[2].rm_eo-pmatch[2].rm_so));
        memcpy(result->netloc,&str[pmatch[2].rm_so],pmatch[2].rm_eo-pmatch[2].rm_so);

        result->path = (char*)malloc(sizeof(char)*(pmatch[3].rm_eo-pmatch[3].rm_so));
        memcpy(result->path,&str[pmatch[3].rm_so],pmatch[3].rm_eo-pmatch[3].rm_so);

        result->query = (char*)malloc(sizeof(char)*(pmatch[4].rm_eo-pmatch[4].rm_so));
        memcpy(result->query,&str[pmatch[4].rm_so],pmatch[4].rm_eo-pmatch[4].rm_so); 

        result->fragment = (char*)malloc(sizeof(char)*(pmatch[5].rm_eo-pmatch[5].rm_so));  
        memcpy(result->fragment,&str[pmatch[5].rm_so],pmatch[5].rm_eo-pmatch[5].rm_so);

        return result; 
    }else{
        printf("%s : do not match\n",str);
    }
    return NULL;
}

void free_url(url_t* url){
    free(url->scheme);
    free(url->netloc);
    free(url->path);
    free(url->query);
    free(url->fragment);
    free(url);
}

int http_program(char * str){
    char* sup_schemes[] = {"http","https"};
    url_t* result = url_parse(str);
    if(result != NULL){
        if(strcmp(result->scheme,sup_schemes[0]) != 0 && strcmp(result->scheme,sup_schemes[1]) != 0){
            perror("Scheme must be one of http or https");
            free_url(result);
            return -1;
        }
        if(result->netloc == NULL){
            perror("Host must be non-empty");
            free_url(result);
            return -1;
        }
    }else{
        return -1;
    }
    // printf("%s\n",str);
    // printf("%s\n",result->scheme);
    // printf("%s\n",result->netloc);
    // printf("%s\n",result->path);
    // printf("%s\n",result->query);
    // printf("%s\n",result->fragment);
    free(result);
    return 1;
}

// ?
int is_valid_url(char* str){
    int result = http_program(str);
    if(result == -1){
        return -1;
    }else{
        return 1;
    }
}