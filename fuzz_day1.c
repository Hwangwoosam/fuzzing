#define EPSILON 1e-8
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

void assertEquals(double x,double y){
    double temp = abs(x-y);
    assert(temp < EPSILON);
}

typedef struct pair{
    double x;
    double y;
    int flag_x;
    int flag_y;
}pair_t;

double my_sqrt(double x){
    double approx = 0;
    double guess = x/2;
    while(approx != guess){
        approx = guess;
        guess = (approx + x/approx)/2;
    }
    return approx;
}

double my_sqrt_log(double x){
    double approx = 0;
    double guess = x/2;
    while(approx != guess){
        printf("approx = %f\n",approx);
        approx = guess;
        guess = (approx + x/approx)/2;
    }
    return approx;
}

double my_sqrt_checked(double x){
    double root = my_sqrt(x);
    assertEquals(root*root,x);
    return root;
}

void sqrt_program(char* x){
    size_t size = strlen(x);
    int flag = 0;
    if(size == 0){
        flag = 1;
    }else{
        for(int i = 0; i < (int)size ; i++){
            if(x[i] == '.'){
                continue;
            }
            if(x[i] < '0' || x[i] > '9'){
                flag = 1;
                break;
            }
        }
    }
    if(flag == 1){
        printf("Illegal Input\n");
    }else{
        double temp = atof(x);
        if(temp < 0){
            printf("Illegal Number\n");
        }else{
            printf("the root of %f is %f\n",temp,my_sqrt(temp));
        }
    }
}

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

double my_sqrt_fixed(double x){
    assert(0 < x);
    if(x == 0){
        return 0;
    }
    return my_sqrt(x);
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

pair_t quadratic_solver(int a,int b,int c){
    pair_t temp;
    if(a == 0){
        if(b == 0){
            if(c == 0){
                temp.x = 0;
                temp.flag_x = 0;
                temp.flag_y = 1;
                return temp; 
            }else{

                temp.flag_x = 1;
                temp.flag_y = 1;
                return temp;
            }
        }else{
            temp.x = -c/b;
            temp.flag_x = 0;
            temp.flag_y = 1;
            return temp;
        }
    }
    double q = b*b - 4*a*c;
    if(q < 0){
        temp.flag_x = 1;
        temp.flag_y = 1;
        return temp;
    }
    if(q == 0){
        temp.x = -b/(2*a);
        temp.flag_x = 0;
        temp.flag_y = 1;
        return temp;
    }
    double solution_1 = (-b + my_sqrt_fixed(q)) /(2*a);
    double solution_2 = (-b + my_sqrt_fixed(q)) /(2*a);
    temp.x = solution_1;
    temp.y = solution_2;
    return temp; 
}

void quadratic_printf(pair_t result){
    if(result.flag_x == 0 && result.flag_y == 1){
        printf("(%f, None)\n",result.x);
    }else if(result.flag_x == 1 && result.flag_y == 1){
        printf("(None, None)\n");
    }else{
        printf("(%f, %f)\n",result.x,result.y);
    }
}

int main(int argc,char* argv[]){

    /*
    double result = my_sqrt(4);
    double expected_result = 2.0;
    if( result == expected_result){
        printf("Test passed\n");
    }else{
        printf("Test failed\n");
    }
    */


    //Generating test
    /*
    srand(time(NULL));
    clock_t start,end;
    start = clock();
    for(int i = 0; i <= 1000; i++){
        int random = rand()%10000;
        assertEquals(my_sqrt(random)*my_sqrt(random),random);
    }
    end = clock();
    printf("%f excute time\n",(float)(end-start)/CLOCKS_PER_SEC);
    */
    //Run-Time verification
    /*
    double x = my_sqrt_checked(2.0);
    printf("%f \n",x);
    */

    // int arr1[] = {5,1,4,2,3,7};
    // int arr2[] = {1,4,5,7,3,2};
    // int *tmp = shellsort(arr1,sizeof(arr1)/sizeof(int));
    // printf("%ld size\n",sizeof(*tmp));
    // // printf("%ld %ld size\n",sizeof(arr1)/sizeof(int),sizeof(tmp)/sizeof(int));
    // if(!is_sorted(tmp,sizeof(tmp)/sizeof(int))){
    //      printf("it's not sorted\n");
    // }
    // if(!is_permutation(arr1,sizeof(arr1)/sizeof(int),tmp,sizeof(tmp)/sizeof(int))){
    //      printf("it's not same array\n");
    // }



    //Exercise2
    /*
    srand(time(NULL));
    for(int i =0; i <1000; i++){
        int num = rand()%100;
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
    */

   //Exercise3
   //1000�� 0.015625 sec
    unsigned long long chance = 1;
    for(int i = 0; i < 64; i++){
        chance *= 2;
    }
    printf("%lld chace\n",chance);
    
    // // float tmp =  __FLT_MAX__;
    // tmp = tmp*1000;
    // printf("%f infinity\n",my_sqrt_fixed(tmp));
    return 0;
}