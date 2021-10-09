#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char s[2048];
    scanf("%s",s);
    if(strlen(s) > 0 && s[0] == 'b'){
        if(strlen(s) > 1 && s[1] == 'a'){
            if(strlen(s) > 2 && s[2] =='d'){
                if(strlen(s) > 3 && s[3] == '!'){
                    perror("Bad!\n");
                }
            }
        }
    }
    printf("hello\n");
    return 0;
}