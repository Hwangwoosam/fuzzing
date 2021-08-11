#include "runner.h"


int main(){
    Runner runner;
    Runner_init(&runner);
    char input[] = "";

    runner.run(&runner,input,sizeof(input));
    
    // int run_checker = runner.run(&runner.result,input,sizeof(input));

    // if(run_checker == -1){
    //     printf("Runner Class memcpy failed in run function\n");
    //     return 0;
    // }

    printf("%s\n",runner.outcome);
    for(int i = 0; i < runner.inp_size;i++){
        printf("%d : %c\n",i,runner.input[i]);
    }
    printf("\n");
    runner.free_inp(runner);
}