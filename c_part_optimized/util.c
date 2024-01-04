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
    if(arg%2 == 0){
        a[3] = 0;
    }else{
        a[3] = 1;
    }
    a[1] = arg;
    a[2] = arg + arg/2;
    return 0;
}
