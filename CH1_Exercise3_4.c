#define EPSILON 1e-8
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

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

double my_sqrt_fixed(double x){
    assert(0 <= x);
    if( x==0){
        return 0;
    }
    return my_sqrt(x);
}

pair_t quadratic_solver(double a,double b,double c){
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

int main(){
    
    //Exercise3
    printf("Testcase of Exercise3\n");
    // a  == 0, b == 0  --> q == 0
    quadratic_printf(quadratic_solver(0,0,1));

    // a == 0 , b > 0   --> q >  0
    quadratic_printf(quadratic_solver(0,1,1));

    // a == 0 , b < 0   --> q >  0
    quadratic_printf(quadratic_solver(0,-1,1));

    // c > 0
    // a > 0 , b > 0   --> q < 0
    quadratic_printf(quadratic_solver(1,1,1)); 
    
    // c < 0 
    // a > 0 , b > 0   --> q > 0
    quadratic_printf(quadratic_solver(1,1,-1));
    
    //part 3
    unsigned long long chance = 1;
    chance = pow(2,63);
    double testtime = 1000000000;
    double result = chance/testtime;
    double minutes = result/60;
    double hour = minutes/60;
    double day = hour/24;
    double year = day*2/365;
    printf("%f take long\n",year);
    
    //Exercise 4
    printf("\nTestcase of Exercise 4\n");
    float tmp =  __FLT_MAX__;
    tmp = tmp*1000;
    printf("%f size\n",tmp);
    printf("%f infinity\n",my_sqrt_fixed(tmp));
}