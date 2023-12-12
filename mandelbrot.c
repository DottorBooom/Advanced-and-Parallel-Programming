#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "mandelbrot.h"

float * mandelbrot_set(int * x, float * row, float * col, float r){

    float * min_n = (float *)malloc(x[1]*x[2]*sizeof(float));
    float complex z;
    float complex fz;
    bool flag = true;

    for(int i = 0; i < x[1]; i++){
        for(int j = 0; j < x[2]; j++){
            z = row[i] + col[j]*I;
            fz = 0;
            for(int k = 0; k < x[1]; k++){
                fz = cpowf(fz,2) + z;
                if(cabsf(fz) >= r){
                    flag = false;
                    min_n[x[2]*i+j] = k+1;
                    break;
                }
            }
            if(flag == true){
                min_n[x[2]*i+j] = x[0];
            }else{
                flag = true;
            }
        }
    }

    printf("\n");
    for (int i = 0; i < x[1]; i++) {
        for (int j = 0; j < x[2]; j++) {
            printf("%2.3f, ", min_n[i * x[2] + j]);
        }
        printf("\n");
    }

    return min_n;
}