// Davide Martinelli SM3201226
#include <stdlib.h>
#include <complex.h>
#include <omp.h>
#include <stdint.h>

uint16_t abs_check(float complex z, float complex fz, uint16_t M){
    for(uint16_t k = 0; k < M; k++){
        fz = cpowf(fz,2) + z;
        if(cabsf(fz) >= 2){
            return k+1;
        }
    }
    return M;
}

uint16_t * mandelbrot_set(int * components, float * y, float * x){

    uint16_t * min_n = (uint16_t *)malloc((components[1]/2+1)*components[2]*sizeof(uint16_t));
    
    //#pragma omp parallel num_threads(omp_get_max_threads())
    //{
    float complex z;
    float complex fz;    
    #pragma omp parallel for schedule(dynamic) collapse(2)
    for(int i = 0; i < components[1]/2+1; i++){
        for(int j = 0; j < components[2]; j++){
            z = y[j] + x[i]*I;
            fz = 0; 
            min_n[components[2]*i+j] = abs_check(z,fz,components[0]);
            
        }
    }
    //}
    return min_n;
}
