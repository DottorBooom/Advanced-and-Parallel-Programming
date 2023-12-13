// Davide Martinelli SM3201226
#include <stdlib.h>
#include <limits.h>
#include <string.h> 
#include <errno.h>

int argv_check(int argc, char * argv[], int * a){

    if (argc != 4){
        return -1;
    }

    if (strlen(argv[2]) == 0 || strlen(argv[3]) == 0) {
        return -2;
    }
    char* p;
    errno = 0;
    long arg1 = strtol(argv[2], &p, 10);
    long arg2 = strtol(argv[3], &p, 10);
    
    if (*p != '\0' || errno != 0) {
        return -3;
    }

    if (arg1 < INT_MIN || arg1 > INT_MAX || arg2 < INT_MIN || arg2 > INT_MAX) {
        return -4;
    }
    int arg = arg1;
    a[0] = arg;
    arg = arg2;
    a[1] = arg;
    a[2] = arg + arg/2;
    return 0;
}

float * linspace(float a, float b, int n){

    float * u = (float *)malloc(n*sizeof(float));
    float c;
    
    if(n < 2 || u == 0)
        return (void*)0;
    
    c = (b - a)/(n - 1);

    for(int i = 0; i < n - 1; ++i)
        u[i] = a + i*c;

    u[n - 1] = b;

    return u;
}