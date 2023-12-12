#include <stdio.h>
#include <stdlib.h>

#include "util.h"
#include "mandelbrot.h"

int main(int argc, char * argv[]){

    //--------------------------------------------------------------//
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
    //--------------------------------------------------------------//
    printf("Creating an equalspaced array for real part and immaginary part...");
    float * r_rows = linspace(-2,1,components[1]);
    float * i_cols = linspace(1,-1,components[2]);
    if(r_rows == 0 || i_cols == 0){
        printf("...program end with error -5\n");
        return 0;
    }
    printf("...arrays line spaced correctly.\n");
    //--------------------------------------------------------------//
    printf("Start computing the mandelbrot set...");
    float r = 2;
    float * min_n = mandelbrot_set(components,r_rows,i_cols,r);

    printf("...values computed correctly.\n");

    //--------------------------------------------------------------//
    printf("End of the program.\n");
    return 0;
}
