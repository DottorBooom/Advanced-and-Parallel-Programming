#ifndef _NETPBM_H
#define _NETPBM_H

#include <stdlib.h>
#include <limits.h>
#include <string.h> 
#include <errno.h>

int argv_check(int argc, char * argv[], int * a);

float * linspace(float a, float b, int n);

#endif