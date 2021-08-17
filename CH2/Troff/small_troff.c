#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int backslash_D(char* str, int size){
    for(int i = 0; i < size -2 ; i++){
        if(str[i] =='\\' && str[i+1] =='D'){
            if(str[i+2] >=0 && str[i+2] < 32){
                perror("there is non-printable character after backslash_D\n");
                return 1;
            }    
        }
    }
    return 0;
}

int no_8th_bit(char* str, int size){
    for(int i = 0; i <size -1; i++){
        if(str[i] >127 || str[i] < 0){
            if(str[i+1] == '\n'){
                perror("no 8th bit\n");
                return 1;
            }
        }
    }
    return 0;
}

int no_dot(char* str, int size){
    for(int i = 0; i <size -1 ; i++){
        if(str[i] =='.'){
            if(str[i+1] == '\n'){
                perror("no dot\n");
                return 1;
            }
        }
    }
    return 0;
}

int main(){
    char buf[1024];
    int s;
    int flag_back= 0;
    int flag_no_8th_bit = 0;
    int flag_no_dot = 0;   
    while((s = read(0,buf,1023)) > 0){
        write(1,buf,s);
        flag_back += backslash_D(buf,s);
        flag_no_8th_bit += no_8th_bit(buf,s);
        flag_no_dot += no_dot(buf,s);
    }
    if(flag_back > 0){
        exit(1);
    }else if(flag_no_8th_bit > 0){
        exit(2);
    }else if(flag_no_dot > 0){
        exit(3);
    }
}