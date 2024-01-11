// Davide Martinelli SM3201226
#ifndef _MANDELBROT_h
#define _MANDELBROT_H

#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <stdint.h>

uint16_t * mandelbrot_set(int * components, float * y, float * x);

uint16_t abs_check(float complex c, uint16_t M);

#endif