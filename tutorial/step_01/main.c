/*
 * Step 1: List All Arguments
 *
 * Topics:
 *  * Compile a single C file by using the GNU Compiler Collection (gcc)
 *
 * To compile and link this program call:
 * 
 * $ gcc -o main main.c
 */

#include <stdio.h>


int main(int argc, char *argv[])
{
    printf("Arguments: %i\n", argc);
    for(int i = 0; i < argc; i++) {
        printf(" * %2i: '%s'\n", i, argv[i]);
    }
}
