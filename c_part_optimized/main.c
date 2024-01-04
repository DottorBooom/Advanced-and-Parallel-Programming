// Davide Martinelli SM3201226
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <omp.h>
#include <stdint.h>

#include "pgm.h"
#include "util.h"

int time_stamp(int * components, int repetitions, char * name){
    
    double t = 0;
    
    for (int i = 0; i < repetitions; i++) {
        
        double start = omp_get_wtime();

        pgm image;
        int8_t err = empty_image(strcat(name,".pgm"), &image, components[2], components[1]);
        if (err != 0) {
            printf("Unable to open image. Program end with error %d.\n", err);
            free(components);
            return err;
        }

        print_image(components, &image);

        err = close_image(&image);
        if(err != 0){
            printf("Unable to close image correctly. Program end with error %d.\n", err);
            free(components);
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
