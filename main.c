// Davide Martinelli SM3201226
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "util.h"
#include "mandelbrot.h"
#include "pgm.h"

int main(int argc, char * argv[]){

    //--------------------------------------------------------------//
    printf("Program started. Computing input arguments...");

    int * components = (int *)malloc(3 * sizeof(int));
    int err = argv_check(argc,argv,components); //divido e variabili singole?

    if(err != 0){
        printf("...program end with error %d.\n",err);
        free(components);
        return 0;
    }else{
        printf("...values initialized correctly:\n");
        printf("+ M = %d\n+ Number of rows = %d\n+ Number of columns = %d\n",components[0],components[1],components[2]);
    }
    //--------------------------------------------------------------//
    printf("Creating an equalspaced array for real part and immaginary part...");
    float * imm = linspace(-1,1,components[1]);
    float * real = linspace(-2,1,components[2]);
    if(real == 0 || imm == 0){
        printf("...program end with error -5.\n");
        free(real); //Meglio alla fine?
        free(imm);
        free(components);
        return 0;
    }
    printf("...arrays line spaced correctly.\n");
    //--------------------------------------------------------------//
    printf("Start computing the mandelbrot set..."); //perché non lo esegue? O3?
    float * min_n = mandelbrot_set(components,real,imm);
    free(real); //Meglio alla fine?
    free(imm);
    printf("...values computed correctly.\n");
    //--------------------------------------------------------------//
    printf("Inizialazing image space...");
    pgm image;
    err = empty_image(strcat(argv[1],".pgm"), &image, components[2], components[1]);
    if (err != 0) {
        printf("...unable to open image. Program end with error %d.\n", err);
        free(components);
        free(min_n);
        return 0;
    }else{
        printf("...mapped completed. Rendering pixel...");
    }
    for (int y = 0; y < components[1]; y++) { //Dovrei spostarlo così gestisco l'errore?
        for (int x = 0; x < components[2]; x++) {
                char * c = pixel_at(&image, x, y);
                if (c == NULL) {
                    printf("...error at x = %d y = %d.\n", x, y);
                }else{
                    *c = (int)(255*log(min_n[components[2]*y+x])/(float)log(components[0]));
                }
            }
    }
    printf("...rendering completed. Image saved correctly.\n");
    err = close_image(&image);
    if(err != 0){
        printf("...unable to close image correctly. Program end with error %d.\n", err);
        free(components);
        free(min_n);
        return 0;
    }
    //--------------------------------------------------------------//
    free(components);
    free(min_n);
    printf("End of the program.\n");
    return 0;
}
