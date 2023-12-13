// Davide Martinelli SM3201226
#include <stdlib.h>
#include <complex.h>
#include <stdbool.h>

float * mandelbrot_set(int * components, float * y, float * x){

    float * min_n = (float *)malloc(components[1]*components[2]*sizeof(float));
    float complex z;
    float complex fz;    
    bool flag = true;

    for(int i = 0; i < components[1]; i++){
        for(int j = 0; j < components[2]; j++){
            z = y[j] + x[i]*I;
            fz = 0;
            for(int k = 0; k < components[0] && flag; k++){
                fz = cpowf(fz,2) + z;
                if(cabsf(fz) >= 2){
                    flag = false;
                    min_n[components[2]*i+j] = k+1;
                }
            }
            if(flag){
                min_n[components[2]*i+j] = components[0]; //più veloce questo o int k?
            }else{
                flag = true;
            }
        }
    }
    return min_n;
}