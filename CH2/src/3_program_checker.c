#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "../include/3_program_checker.h"

item_list_t* list_init(){
    item_list_t* temp = (item_list_t*)malloc(sizeof(item_list_t));
    temp->size = 0;
    temp->list = 0x0;
    return temp;
}


int code_repOK(char* code){
    assert(strlen(code) == 3);
    for(int i = 0; i < 3; i++){
        assert(isalpha(code[i]) > 0);
        assert(isupper(code[i]) > 0);
    }

    return 1;
}

int airport_codes_repOK(item_list_t* temp_list){
    for(int i =0; i < temp_list->size;i++){
        assert(code_repOK(temp_list->list[i]->key));   
    }
    return 1;
}

void add_item(item_list_t* i_list,char* key,char* value){
    assert(code_repOK(key));
    assert(airport_codes_repOK(i_list));
    i_list->size++;
    if(i_list->size == 1){
        i_list->list = (item_t**)malloc(sizeof(item_t*)*i_list->size);
    }else{
        i_list->list = (item_t**)realloc(i_list->list,sizeof(item_t*) * i_list->size);
    }
    i_list->list[i_list->size-1] = (item_t*)malloc(sizeof(item_t));
    i_list->list[i_list->size-1]->key =(char*)malloc(sizeof(char)*(strlen(key)));
    i_list->list[i_list->size-1]->value =(char*)malloc(sizeof(char)*(strlen(value)));

    strcpy(i_list->list[i_list->size-1]->key,key);
    strcpy(i_list->list[i_list->size-1]->value,value);
    
    assert(airport_codes_repOK(i_list));
}

void delet_list(item_list_t* i_list){
    for(int i = 0; i < i_list->size;i++){
        free(i_list->list[i]->key);
        free(i_list->list[i]->value);
        free(i_list->list[i]);
    }
    free(i_list->list);
    free(i_list);
}