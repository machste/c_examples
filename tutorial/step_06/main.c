/*
 * Step 6: Put Functions to Different Files
 *
 * Topics:
 *  * Create header files
 *  * Include guard
 *  * Define global functions
 *
 * To compile and link this program call:
 *
 * $ gcc -c arg.c
 * $ gcc -c math.c
 * $ gcc -o main main.c arg.o math.o
 */

#include <stdio.h>

#include "arg.h"


int main(int argc, char *argv[])
{
    printf("Arguments: %i\n", argc);
    for(int i = 0; i < argc; i++) {
        Arg arg = arg_init(i, argv[i]);
        arg_print(&arg);
    }
}
