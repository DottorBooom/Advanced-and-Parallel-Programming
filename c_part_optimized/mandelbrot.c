// Davide Martinelli SM3201226
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

uint16_t abs_check(float x0, float y0,uint16_t M){
    float xn;
    float x = 0;
    float y = 0;
    for(uint16_t k = 0; k < M; k++){
        xn = x * x - y * y + x0;
        y = 2 * x * y + y0;
        x = xn;
        if (sqrt(x * x + y * y) >= 2) {
            return k+1;
        }
    }
    return M;
}
