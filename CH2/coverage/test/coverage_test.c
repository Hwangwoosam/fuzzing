#include "../include/coverage.h"

int main(int argc,char* argv[]){   
    
    make_gcov(argc,argv);
    read_gcov(argv[1]);

    return 0;
}