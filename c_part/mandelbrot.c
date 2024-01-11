// Davide Martinelli SM3201226
//--------------------------------------------------------------//
//                        LIBRARIES                             //
//--------------------------------------------------------------//
#include <stdlib.h>
#include <complex.h>
#include <omp.h>
#include <stdint.h>
//--------------------------------------------------------------//
//                            CODE                              //
//--------------------------------------------------------------//
uint16_t abs_check(float complex c, uint16_t M){
/* This function calculates the value of n for which the modulus of the series is
   greater than or equal to 2 and returns the value. If it never surpasses it, it 
   returns the maximum number of allowed iterations. As argument it takes a complex
   number c (that is suppose to be summed every iteration) and the max iteration M 
   (stored in an unsigned integer with 16bit = 2^16 ~ 65.000).
*/
    float complex z = 0; // The base case corrispond with a z = 0.
    for(uint16_t k = 0; k < M; k++){ 
        z = cpowf(z,2) + c; // Every cycle we set z equal to the power of himself plus a c value.
        if(cabsf(z) >= 2){ // Then, if the absolute value of z is over or equal to 2.
            return k+1; // It return the k+1-th operation (because we start from 0, and not from 1).
        }
    }
    return M; // Otherwise, it return the M-th operations
}

uint16_t * mandelbrot_set(int * components, float * y, float * x){
/*  This function 'scrolls' through the space of intervals as if it were a matrix 
    (which will be the actual representation of the image), passing the complex 
    number c to the 'abs_check' function to calculate the degree of membership of the 
    series to the Mandelbrot set.
*/
    uint16_t * min_n = (uint16_t *)malloc((components[1]/2+1)*components[2]*sizeof(uint16_t)); // The memroy space that will be returned to the main file
    
    float complex c;
    #pragma omp parallel for schedule(dynamic) collapse(2)
    // In the next for we make an intresting shortcut. Basically the Mandelbrot fractal is simmetric at y=0. 
    // Then, we just need half of the set for print the complete image.
    for(int i = 0; i < components[1]/2+1; i++){ 
        for(int j = 0; j < components[2]; j++){
            c = y[j] + x[i]*I;
            min_n[components[2]*i+j] = abs_check(c,components[0]);
            
        }
    }
    return min_n;
}