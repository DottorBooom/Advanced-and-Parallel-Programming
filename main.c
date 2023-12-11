#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int main(int argc, char * argv[]){

    printf("Program started. Computing input arguments...");

    int * components = (int *)malloc(3 * sizeof(int));
    int err = argv_check(argc,argv,components);

    if(err != 0){
        printf("...program end with error %d\n",err);
        return 0;
    }else{
        printf("...values initialized correctly:\n");
        printf("+ M = %d\n+ Number of rows = %d\n+ Number of columns = %d\n",components[0],components[1],components[2]);
    }

    printf("Creating an equalspaced array for real part and immaginary part...");
    float * r_nums = linspace(-2,1,components[1]);
    float * r_cols = linspace(-1,1,components[2]);
    if(r_nums == 0 || r_cols == 0){
        printf("...program end with error -5\n");
        return 0;
    }
    printf("...arrays line spaced correctly.\n");

    printf("End of the program.\n");
    return 0;
}
