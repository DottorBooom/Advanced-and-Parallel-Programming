// Davide Martinelli SM3201226
//--------------------------------------------------------------//
//                        LIBRARIES                             //
//--------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <omp.h>
#include <stdint.h>
//--------------------------------------------------------------//
//                    HOME MADE LIBRARIES                       //
//--------------------------------------------------------------//
#include "util.h"
#include "mandelbrot.h"
#include "pgm.h"
//--------------------------------------------------------------//
//                            CODE                              //
//--------------------------------------------------------------//
int time_stamp(int * components, int repetitions, char * name){
/* "The task of this function is to calculate the average execution time over a 
    given number of iterations passed as a parameter (repetitions). It also takes as 
    input a pointer to a memory space called components, which contains all the values 
    necessary to compute the fractal, and the name to give to the image."
*/
    double t = 0; // Variable required to calculate the average execution time.
    
    //The main for loop that performs the fractal computation multiple times, equal to the value of repetitions.
    for (int i = 0; i < repetitions; i++) { 
        
        double start = omp_get_wtime(); // Using the given function, we catch the start time.

        //--------------------------------------------------------------//

        /* Using the function declared in the 'Util.h' library, we make it return a 
            memory space containing a number of intervals ([-1,1] for the immaginary 
            part, and [-2,1] for the real part) equal to the x/y size of the image.
        */
        float * imm = linspace(-1,1,components[1]); 
        float * real = linspace(-2,1,components[2]);
        
        // A quick check for verify if the linespace was executed correctly.
        if(real == 0 || imm == 0){ // If not, we print the number error, free all the malloc and then return to the main function.
            printf("Program end with error -5.\n");
            free(real);
            free(imm);
            free(components);
            return -5;
        }

        //--------------------------------------------------------------//
        
        /* We use the function implemented in the 'mandelbrot.h' library to return a 
            memory space (formatted as 16-bit unsigned integers) containing the 
            calculated Mandelbrot set. As a parameter, we pass the two matrices containing 
            the intervals and components.
        */
        uint16_t * min_n = mandelbrot_set(components,real,imm); 
        // Then, we can free the memory space used to evenly space the intervals, because
        // we don't need them anymore.
        free(real); 
        free(imm);
        
        //--------------------------------------------------------------//

        // All the following functions are derived from the 'pgm.h' library.

        pgm image; // Declare the image structure.
        int8_t err = empty_image(strcat(name,".pgm"), &image, components[2], components[1]); // Generate the empy space for the image.
        // A quick check for verify if the memory allocation was completed correctly.       
        if (err != 0) { // If not, we print the number error, free all the malloc and then return to the main function.
            printf("Unable to open image. Program end with error %d.\n", err);
            free(components);
            free(min_n);
            return err;
        }

        // This is the other main function of the program that assigns a value to 
        // each pixel using the set of values previously calculated.
        
        print_image(components, &image, min_n); 

        // A quick check for verify if the free of the memory was completed correctly.  
        if(err != 0){ // If not, we print the number error, free all the malloc and then return to the main function.
            printf("Unable to close image correctly. Program end with error %d.\n", err);
            free(components);
            free(min_n);
            return err;
        }

        double end = omp_get_wtime(); // Using the given function, we catch the start time.
        t += (end - start); // We add the difference beetween the start and the end to t
        printf("Finito test %d\n",i); // And we simply print to terminal that we end the n-th iteretion
    }

    printf("Tempo per esecuzione standard: %f ms\n", 1000 * t / repetitions); // After all n iterations are completed, we print the average
    return 0; 
}

int main(int argc, char * argv[]){
/* This is the main primary function. It is mainly used to receive parameters via the 
    command line and handle errors.
*/

    /* We create a memory space to save 3 integers received via the command line:
        - The number of M operations;
        - The number of rows;
        - The number of columns (n rows * 1.5);
        But first, we have to check if the value are convertible fromt char to int and if they are valid.
    */
    int * components = (int *)malloc(3 * sizeof(int));  
    int8_t err = argv_check(argc,argv,components); 
    
    // A quick check for verify if the values were computable.
    if (err != 0) { // If not, we print the number error, free all the malloc and then return to the main function.
        printf("Program end with error %d.\n",err);
        free(components);
        return 0;
    }

    //--------------------------------------------------------------//
    
    int repetitions = 1; // A variable for chosse the number of iterations
    // The next function is the real 'main' function of the program. It execute all the 
    // calculus and the rendering of the image. And then it return a value, 0 for a good execution, 
    // otherwise a specific error for every problem that can occur.
    err = time_stamp(components, repetitions, argv[1]); 

    // A quick check for verify if the values were computable.
    if (err != 0) { // If not, we print the number error, free all the malloc and then return to the main function.
        free(components);
        return 0;
    }
    //--------------------------------------------------------------//

    // If everything went good, we can then free the components space memory, print the end of the program and return 0.
    free(components);
    printf("End of the program.\n");
    return 0;
}
