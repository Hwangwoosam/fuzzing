#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/create_inp.h"
#include "../include/config.h"


#define INTERESTING_8 \
  -128,          /* Overflow signed 8-bit when decremented  */ \
  -1,            /*                                         */ \
   0,            /*                                         */ \
   1,            /*                                         */ \
   16,           /* One-off with common buffer size         */ \
   32,           /* One-off with common buffer size         */ \
   64,           /* One-off with common buffer size         */ \
   100,          /* One-off with common buffer size         */ \
   127           /* Overflow signed 8-bit when incremented  */

#define INTERESTING_16 \
  -32768,        /* Overflow signed 16-bit when decremented */ \
  -129,          /* Overflow signed 8-bit                   */ \
   128,          /* Overflow signed 8-bit                   */ \
   255,          /* Overflow unsig 8-bit when incremented   */ \
   256,          /* Overflow unsig 8-bit                    */ \
   512,          /* One-off with common buffer size         */ \
   1000,         /* One-off with common buffer size         */ \
   1024,         /* One-off with common buffer size         */ \
   4096,         /* One-off with common buffer size         */ \
   32767         /* Overflow signed 16-bit when incremented */

#define INTERESTING_32 \
  -2147483648LL, /* Overflow signed 32-bit when decremented */ \
  -100663046,    /* Large negative number (endian-agnostic) */ \
  -32769,        /* Overflow signed 16-bit                  */ \
   32768,        /* Overflow signed 16-bit                  */ \
   65535,        /* Overflow unsig 16-bit when incremented  */ \
   65536,        /* Overflow unsig 16 bit                   */ \
   100663045,    /* Large positive number (endian-agnostic) */ \
   2147483647    /* Overflow signed 32-bit when incremented */

static int8_t  interesting_8[] = { INTERESTING_8 } ;
static int16_t interesting_16[] = { INTERESTING_8 ,INTERESTING_16 } ;
static int32_t interesting_32[] = { INTERESTING_8 ,INTERESTING_16 ,INTERESTING_32 } ;

void create_inp(char* random_inp,int * inp_size,input_arg_t inp_config){
    int string_length = rand()%(inp_config.f_max_len+1);
    *inp_size = string_length; 
    for(int i = 0; i < string_length; i++){
        char tmp = rand()%(inp_config.f_char_range + 1) + inp_config.f_char_start;
        random_inp[i] = tmp;
    }
    random_inp[string_length] = '\0';
    return;
}
//############################
// single bit operation
//############################

int flip_random(char* str, char* target ,int str_size){
    if(str_size <= 0){
        perror("input is NULL");
        return -1;
    }

    if(memcpy(target,str,str_size) == NULL){
        perror("flip memcpy failed\n");
        return -1;
    }

    int bit_size[3] = {1,2,4};
    int times = 0;
    for(int i = 0; i < 3; i++){
        if(bit_size[2-i] < str_size){
            times = rand()%(3-i);
            break;
        }else{
            continue;
        }       
    }
    for(int i = 0; i < bit_size[times]; i++){
        int pos = rand()%str_size;
        char c = str[pos];
        char bit = 1 << rand()%7;
        char new_c = c^bit;
        target[pos] = new_c;
    }

    target[str_size-1] ='\0';
    return str_size;
} 

//#############################
//byte operator
//#############################

int delete_random(char* str,char* target, int str_size,int byte){
    if(str_size <= 0){
        perror("input is NULL");
        return -1;
    }

    int pos;
    if(str_size == byte){
        pos = 0;
    }else{
        pos = rand()%(str_size - byte);
    }

    if(memcpy(target,str,pos) == NULL){
        perror("delete 1 memcpy failed\n");
        return -1;
    }

    if(memcpy(target + pos,str + byte + pos , str_size-pos-byte + 1) == NULL){
        perror("delete 2 memcpy failed\n");
        return -1;
    }

    target[str_size-byte - 1] ='\0';
    return str_size - byte;
}

int insert_random(char* str, char* target, int str_size,int byte){
    int operator = rand()%3;
    int length;
    switch (operator)
    {
    case 0:
        
        length = random_value(str,target,str_size,byte);
        return length;
    case 1:

        length = simple_arithmatic_insert(str,target,str_size,byte);
        return length;
    case 2:
        
        length = known_integers_insert(str,target,str_size,byte);
        return length;
    default:
        break;
    }
}

int change_random(char* str, char* target, int str_size,int byte){
    int operator = rand()%4;
    int length;

    switch (operator)
    {
    case 0:
        
        length = byte_flip(str,target,str_size,byte);
        return length;
    case 1:
        
        length = simple_arithmatic_change(str,target,str_size,byte);
        return length;
    case 2:
        
        length = known_integers_change(str,target,str_size,byte);
        return length;
    case 3:
        
        length = flip_random(str,target,str_size);
        return length;
    default:
        break;
    }
}

int random_value(char* str, char* target, int str_size, int byte){
    if(str_size <= 0){
        perror("input is NULL");
        return -1;
    }

    int pos;
    pos = rand()%str_size;

    if(memcpy(target,str,pos) == NULL){
        perror("insert 1 memcpy failed\n");
        return -1;
    }

    for(int i = 0; i < byte; i++){
        char random_c = rand()%96 + 32;
        target[pos + i] = random_c; 
    }

    if(memcpy(target + pos + byte, str+pos ,str_size-pos) == NULL){
        perror("insert 3 memcpy failed\n");
        return -1;
    }

    target[str_size + byte - 1] ='\0';
    return str_size + byte;
}

int byte_flip(char* str, char* target ,int str_size,int byte){
    if(str_size <= 0){
        perror("input is NULL");
        return -1;
    }

    if(memcpy(target,str,str_size) == NULL){
        perror("flip memcpy failed\n");
        return -1;
    }

    char new_char;
    int pos;
    if(str_size == byte){
        pos = 0;
    }else{
        pos = rand()%(str_size-byte);
    }
    
    for(int i = 0; i < byte ; i++){
        new_char = (char) str[pos + i] ^ 0xff;
        target[pos + i] = new_char;
   }
    target[str_size - 1] = '\0';
    return str_size;
}

int simple_arithmatic_insert(char* str,char* target,int str_size,int byte){
    if(str_size <= 0){
        perror("input is NULL");
        return -1;
    }

    int pos = rand()%str_size;
    if(memcpy(target,str,pos) == NULL){
        perror("simple arithmatic insert memcpy failed\n");
        return -1;
    }
    for(int i = 0; i < byte ; i++){
        int val = rand()%71 - 35;
        target[pos + i] = (str[pos] + val);
    }
    if(memcpy(target + pos + byte , str + pos , str_size - pos) == NULL){
        perror("simple arithmatic insert2 memcpy failed\n");
        return -1;
    }
    target[str_size + byte - 1] = '\0';
    return str_size + byte;
}

int simple_arithmatic_change(char* str,char* target,int str_size,int byte){
    if(str_size <= 0){
        perror("input is NULL");
        return -1;
    }
    int pos;
    if(str_size == byte){
        pos = 0;
    }else{
        pos = rand()% (str_size-byte);
    }
    if(memcpy(target,str,pos) == NULL){
        perror("simple arithmatic insert memcpy failed\n");
        return -1;
    }
    for(int i = 0; i < byte ; i++){
        int val = rand()%71 - 35;
        target[pos + i] = (str[pos + i] + val);
    }
    if(memcpy(target + pos + byte , str + pos + byte , str_size - pos - byte) == NULL){
        perror("simple arithmatic insert2 memcpy failed\n");
        return -1;
    }
    target[str_size - 1] = '\0';
    return str_size;
}


int known_integers_insert(char* str, char* target ,int str_size,int byte){

    if(str_size <= 0){
        perror("input is NULL");
        return -1;
    }

    int known_integer[] = { 9, 19 ,27};
    int pos = rand()%str_size;
    int category = rand()%3;
    int index = rand()%known_integer[category];
    if(memcpy(target,str,pos) == NULL){
        perror("flip memcpy failed\n");
        return -1;
    }

    int check;
    switch (category)
    {
    case 0:
        *(int8_t*)(target + pos) = interesting_8[index];
        break;
    
    case 1:
        *(int16_t*)(target + pos) = interesting_16[index];
        break;
    case 2 :
        *(int32_t*)(target + pos) = interesting_32[index];
        break;
    default:
        break;
    }

    if(memcpy(target + pos + byte, str + pos, str_size - pos +1 ) == NULL){
        perror("flip memcpy failed\n");
        return -1;
    }
    target[str_size + byte - 1] = '\0';
    return str_size + byte;
}

int known_integers_change(char* str, char* target ,int str_size,int byte){
    if(str_size <= 0){
        perror("input is NULL");
        return -1;
    }

    if(memcpy(target,str,str_size) == NULL){
        perror("flip memcpy failed\n");
        return -1;
    }

    int known_integer[] = { 9, 10 ,8};
    int pos;
    if(str_size == byte){
        pos = 0;
    }else{
        pos = rand()%(str_size - byte);
    }

    int category = rand()%3;
    int index = rand()%known_integer[category];

    char new_char;
    switch (category)
    {
    case 0:
        *(int8_t*)(target + pos) = interesting_8[index];
        break;
    
    case 1:
        *(int16_t*)(target + pos) = interesting_16[index];
        break;

    case 2 :
        *(int32_t*)(target + pos) = interesting_32[index];
        break;
    default:
        break;
    }

    target[pos] = new_char;
    target[str_size - 1] = '\0';

    return str_size;
}


int mutate(char* str,char* target,int str_size){

    int random_num = rand()%3;

    int byte_size[3] = {1,2,4};
    int byte = 0;
    for(int i = 0; i < str_size ; i++){
        if(byte_size[2-i] <= str_size){
           byte = rand()%(3-i);
           break;
        }
    }

    if(random_num == 0){

       return delete_random(str,target,str_size,byte_size[byte]);

    }else if(random_num == 1){

       return insert_random(str,target,str_size,byte_size[byte]);

    }else{

       return change_random(str,target,str_size,byte_size[byte]);

    }

    return - 1;
}