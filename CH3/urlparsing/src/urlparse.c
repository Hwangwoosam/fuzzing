#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/urlparse.h"

int delete_random(char* str,char* target, int str_size){
    if(str == NULL || str_size <= 0){
        perror("input is NULL");
        return -1;
    }
    int pos = rand()%(str_size);
    if(memcpy(target,str,pos) == NULL){
        perror("delete 1 memcpy failed\n");
        return -1;
    }
    if(memcpy(target + pos, str+pos+1,str_size-pos-1) == NULL){
        perror("delete 2 memcpy failed\n");
        return -1;
    }
    target[str_size-1] ='\0';
    return str_size - 1;
}

int insert_random(char* seed, char* target, int seed_size){
    if(seed == NULL || seed_size <= 0){
        perror("input is NULL");
        return -1;
    }
    int pos = rand()%(seed_size);
    char random_c = rand()%96 + 32;
    if(memcpy(target,seed,pos) == NULL){
        perror("insert 1 memcpy failed\n");
        return -1;
    }
    target[pos] = random_c; 
    if(memcpy(target + pos + 1, seed+pos ,seed_size-pos) == NULL){
        perror("insert 3 memcpy failed\n");
        return -1;
    }
    target[seed_size+1] ='\0';

    return seed_size + 1;
}

int flip_random(char* str, char* target ,int str_size){
    if(str == NULL || str_size <= 0){
        perror("input is NULL");
        return -1;
    }
    int pos = rand()%str_size;
    char c = str[pos];
    char bit = 1 << rand()%7;
    char new_c = c^bit;

    if(memcpy(target,str,pos) == NULL){
        perror("flip 1 memcpy failed\n");
        return -1;
    }
    if(memcpy(target + pos,&new_c,1) == NULL){
        perror("flip 2 memcpy failed\n");
        return -1;
    }
    if(memcpy(target + pos + 1, str+pos + 1,str_size-pos-1) == NULL){
        perror("flip 3 memcpy failed\n");
        return -1;
    }
    target[str_size] ='\0';
    return str_size;
}

int mutate(char* str,char* target,int str_size){

    int random_num = rand()%3;
 
    if(random_num == 0){
       return delete_random(str,target,str_size);

    }else if(random_num == 1){
       return insert_random(str,target,str_size);

    }else{
       return flip_random(str,target,str_size);      
    }


}

int url_parse(char* str,url_t* result){
    regex_t state;
    char url_regex[] = "^([a-zA-Z]*)://([^/?]*)/?([^?]*)\\?\?([^/]*)#?(.*)";
    if(regcomp(&state,url_regex,REG_EXTENDED)){
        perror("regcomp failed\n");
    }
    
    char* info[6];
    regmatch_t pmatch[6];
    int status = regexec(&state,str,6,pmatch,0);
    if(status == 0){
        for(int i = 0; i < 5; i++){
            int length = pmatch[i+1].rm_eo-pmatch[i+1].rm_so;
            if(length >0){
                info[i] = (char*)malloc(sizeof(char)*(length+1));
                memcpy(info[i],str + pmatch[i+1].rm_so,length);
                info[i][length] = 0x0;
            }else{
                info[i] = NULL;
            }
        }

        result->scheme = info[0];
        result->netloc = info[1];
        result->path = info[2];
        result->query = info[3];
        result->fragment = info[4];
        return 1; 
    }else{
        printf("%s : do not match\n",str);
    }
    return -1;
}

void free_url(url_t* url){
    if(url->scheme != NULL){
        free(url->scheme);
    }
    if(url->netloc != NULL){
        free(url->netloc);
    }
    if(url->path != NULL){
        free(url->path);
    }
    if(url->query != NULL){
        free(url->query);
    }
    if(url->fragment != NULL){
        free(url->fragment);
    }
    free(url);
}

int http_program(char * str,url_t* result){
    char* sup_schemes[] = {"http","https"};
    int ret_code = url_parse(str,result);
    if(ret_code == 1){
        if(strcmp(result->scheme,sup_schemes[0]) != 0 && strcmp(result->scheme,sup_schemes[1]) != 0){
            perror("Scheme must be one of http or https");
            return -1;
        }
        if(result->netloc == NULL){
            perror("Host must be non-empty");
            return -1;
        }
    }else{
        return 0;
    }
    return 1;
}

int is_valid_url(char* str,int str_size,url_t* result){
    char* str_cp = (char*)malloc(sizeof(char)*str_size);
    strcpy(str_cp,str);
    int ret_code = http_program(str_cp,result);
    free(str_cp);
    if(ret_code == -1){
        return -1;
    }else if(ret_code == 0){

        return -1;
    }else{
        return 1;
    }
}

void test_mutation(){
    char seed_input[256];
    scanf("%s",seed_input);
    char** valid_inp = (char**)malloc(sizeof(char*)* 20);
    int idx = 0;
    char* inp = (char*)malloc(sizeof(char)*(sizeof(seed_input) + 2));
        int length = mutate(seed_input,inp,sizeof(seed_input));
        url_t* result = (url_t*)malloc(sizeof(url_t));
        int ret_check = is_valid_url(inp,length,result);
        if( ret_check== 1){
            valid_inp[idx] = (char*)malloc(sizeof(char)*(length+1));
            memcpy(valid_inp[idx],inp,length);
            valid_inp[idx][length] ='\0';
            idx++;
        }
        if(ret_check != -1){
            free_url(result);
        }else{
            free(result);
        }
    free(inp);
    for(int i = 0; i < idx; i++){
        printf("%s\n",valid_inp[i]);
        free(valid_inp[i]);
    }
}
int main(){
    srand(time(NULL));
    test_mutation();
    return 0;
}