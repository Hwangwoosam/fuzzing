#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int * shellsort(int* elems,int size,int* new_num){
    int * sorted_elem = (int*)malloc(sizeof(int)*size);
    int num = size;
    *new_num = size;
    for(int i = 0; i < num; i++){
       sorted_elem[i] = elems[i] ;
    }
    int gaps[] = {701,301,132,57,23,10,4,1};
    for(int i = 0; i <8; i++){

        for(int j = gaps[i] ; j < num;j++){
            int temp = sorted_elem[j];
            int k = j;
            while(k >= gaps[i] && sorted_elem[k-gaps[i]] > temp){
                sorted_elem[k] = sorted_elem[k-gaps[i]];
                k -= gaps[i];
            }
            sorted_elem[k] = temp;
        }

    }
    printf("[");
    for(int i = 0; i < num ; i++){
        printf("%d, ",sorted_elem[i]);
    }
    printf("\b\b]\n");

    return sorted_elem;
}

int is_sorted(int * elem,int size){
    for(int i = 0; i < size-1; i++){
        if(elem[i] > elem[i+1]){
            return 0;
        }
    }
    return 1;
}

int is_permutation(int* a,int a_size,int *b,int b_size){
    if(a_size != b_size){
        return 0;
    }
    for(int i = 0; i < a_size;i++){
        int a_num = 0,b_num = 0;
        for(int j = 0; j < a_size; j++){
            if(a[j] == a[i]){
                a_num++;
            }
            if(b[j] == a[i]){
                b_num++;
            }
        }
        if(a_num != b_num){
            return 0;
        }
    }
    return 1;
}

int main(){
    
    srand(time(NULL));

    // array size : 0 ~ 999 , elemnet range 0~ 999;
    for(int i =0; i <1000; i++){
        int num = rand()%1000;
        int temp_arr[num];
        for(int j = 0; j <num; j++){
            temp_arr[j] = rand()%1000;
        }
        int sorted_leng = 0;
        int *sorted_temp = shellsort(temp_arr,num,&sorted_leng);
        if(!is_sorted(sorted_temp,num)){
            printf("it's not sorted\n");
            continue;
        }
        if(!is_permutation(temp_arr,num,sorted_temp,sorted_leng)){
            printf("it's not same array\n");
            continue;
        }
        free(sorted_temp);
    }

    // array size : 0~ 999 , element range -500~500
    for(int i =0; i <1000; i++){
        int num = rand()%1000;
        int temp_arr[num];
        for(int j = 0; j <num; j++){
            temp_arr[j] = rand()%1000 - 500 ;
        }
        int sorted_leng = 0;
        int *sorted_temp = shellsort(temp_arr,num,&sorted_leng);
        if(!is_sorted(sorted_temp,num)){
            printf("it's not sorted\n");
            continue;
        }
        if(!is_permutation(temp_arr,num,sorted_temp,sorted_leng)){
            printf("it's not same array\n");
            continue;
        }
        free(sorted_temp);
    }
    
    // array size : 0~ 999 , element range -1000~0
     for(int i =0; i <1000; i++){
        int num = rand()%1000;
        int temp_arr[num];
        for(int j = 0; j <num; j++){
            temp_arr[j] = rand()%1000 - 1000 ;
        }
        int sorted_leng = 0;
        int *sorted_temp = shellsort(temp_arr,num,&sorted_leng);
        if(!is_sorted(sorted_temp,num)){
            printf("it's not sorted\n");
            continue;
        }
        if(!is_permutation(temp_arr,num,sorted_temp,sorted_leng)){
            printf("it's not same array\n");
            continue;
        }
        free(sorted_temp);
    }
}
