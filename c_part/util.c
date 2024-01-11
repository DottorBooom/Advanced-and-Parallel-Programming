// Davide Martinelli SM3201226
//--------------------------------------------------------------//
//                        LIBRARIES                             //
//--------------------------------------------------------------//
#include <stdlib.h>
#include <limits.h>
#include <string.h> 
#include <errno.h>
//--------------------------------------------------------------//
//                            CODE                              //
//--------------------------------------------------------------//
int argv_check(int argc, char * argv[], int * a){
/*  This function handles command-line inputs. Essentially, it checks if 
    the number of iterations and the size of the image are convertible to a 
    number (preferably an integer) and creates the components array that we 
    will carry throughout the execution of the program.
*/

    if (argc != 4){ // Check if the number of inupt is 4 (name of .exe + name of file + number of iteration + nunber of row)
        return -1;
    }

    if (strlen(argv[2]) == 0 || strlen(argv[3]) == 0) { // Check the lenght of the input 2 and 3
        return -2;
    }
    char* p;
    errno = 0;
    long arg1 = strtol(argv[2], &p, 10);
    long arg2 = strtol(argv[3], &p, 10);
    
    if (*p != '\0' || errno != 0) { // Check if the convertion to long was executed correctly
        return -3;
    }

    if (arg1 < INT_MIN || arg1 > INT_MAX || arg2 < INT_MIN || arg2 > INT_MAX) { // Check if the that long number can be contained in to an int
        return -4;
    }
    // Start the assignement to components
    int arg = arg1; 
    a[0] = arg;
    arg = arg2;
    a[1] = arg;
    a[2] = arg + arg/2; // Number of columns = number of rows * 1.5
    return 0;
}

float * linspace(float a, float b, int n){
/* This function evenly spaces a matrix into n intervals starting from values a and b.
*/

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