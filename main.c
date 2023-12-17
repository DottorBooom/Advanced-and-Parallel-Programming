// Davide Martinelli SM3201226
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <omp.h>
#include <stdint.h>

#include "util.h"
#include "mandelbrot.h"
#include "pgm.h"

int time_stamp(int * components, int repetitions, char * name){
    
    double t = 0;
    
    for (int i = 0; i < repetitions; i++) {
        
        double start = omp_get_wtime();

        float * imm = linspace(-1,1,components[1]);
        float * real = linspace(-2,1,components[2]);
        
        if(real == 0 || imm == 0){
            printf("Program end with error -5.\n");
            free(real);
            free(imm);
            free(components);
            return -5;
        }

        //--------------------------------------------------------------//
        
        uint16_t * min_n = mandelbrot_set(components,real,imm);
        free(real);
        free(imm);
        
        //--------------------------------------------------------------//

        pgm image;
        int8_t err = empty_image(strcat(name,".pgm"), &image, components[2], components[1]);
        if (err != 0) {
            printf("Unable to open image. Program end with error %d.\n", err);
            free(components);
            free(min_n);
            return err;
        }

        print_image(components, &image, min_n);

        err = close_image(&image);
        if(err != 0){
            printf("Unable to close image correctly. Program end with error %d.\n", err);
            free(components);
            free(min_n);
            return err;
        }

        double end = omp_get_wtime();
        t += (end - start);
        printf("Finito test %d\n",i);
    }

    printf("Tempo per esecuzione standard: %f ms\n", 1000 * t / repetitions);
    return 0;
}

int main(int argc, char * argv[]){

    //--------------------------------------------------------------//
    int * components = (int *)malloc(4 * sizeof(int));
    int8_t err = argv_check(argc,argv,components);

    if(err != 0){
        printf("Program end with error %d.\n",err);
        free(components);
        return 0;
    }
    //--------------------------------------------------------------//
    int repetitions = 5;
    err = time_stamp(components, repetitions, argv[1]);
    if(err != 0){
        free(components);
        return 0;
    }
    //--------------------------------------------------------------//
    free(components);
    printf("End of the program.\n");
    return 0;
}
