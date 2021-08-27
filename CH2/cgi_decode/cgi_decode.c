#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "./include/cgi_decode.h"
// %__ he
char hex_to_dec(char* hex_code){
    int dec[2] ={0,};
    
    for(int i = 0; i < 2; i++){
        if(hex_code[i] >= '0' && hex_code[i] <= '9'){
            dec[i] = hex_code[i] -'0';
        
        }else if(hex_code[i] >= 'A' && hex_code[i] <= 'F'){
            dec[i] = hex_code[i] -'A' + 10;
        
        }else if(hex_code[i] >= 'a' && hex_code[i] <= 'f'){
            dec[i] = hex_code[i] -'a' + 10;
        
        }else{
            perror("Wrong hex code\n");
            exit(1);
        }
    }
    
    char decode_c = dec[0]*16 +dec[1];
    return decode_c; 
}


char* cgi_decode(char*str){
    int idx = 0;
    int length = strlen(str);
    char* trans_word = (char*)malloc(sizeof(char)*length);

    for(int i = 0; i < length; i++){
        if(str[i] == '+'){
            trans_word[idx++] =' ';
        }else if(str[i] == '%'){
            char hex_code[2];
            memcpy(hex_code,str+i+1,2);
            trans_word[idx++] = hex_to_dec(hex_code);
            i += 2;
        }else{
            trans_word[idx++] = str[i];
        }
    }
    trans_word[idx] = '\0';
    return trans_word;
}

void black_box(){
    assert(strcmp(cgi_decode("+")," ") == 0 );
    assert(strcmp(cgi_decode("%20")," ") == 0);
    assert(strcmp(cgi_decode("abc"),"abc") == 0);
}
int main(int argc,char* argv[]){
    for(int i =0; i <argc;i++){
        cgi_decode(argv[i]);
    }
    black_box();
    return 0;
}