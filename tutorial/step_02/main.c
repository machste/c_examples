/*
 * Step 2: Is the Argument a Number?
 *
 * Topics:
 *  * Create a local function
 *  * Use of `stdtol()` and `stdtod()` to convert a c-string to number
 *
 * To compile and link this program call:
 * 
 * $ gcc -o main main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


static bool cstr_to_long(const char *cstr, long *l)
{
    char *endptr;
    long _long = strtol(cstr, &endptr, 0);
    if (l != NULL) {
        *l = _long;
    }
    return *endptr == '\0';
}

static bool cstr_to_double(const char *cstr, double *d)
{
    char *endptr;
    double _double = strtod(cstr, &endptr);
    if (d != NULL) {
        *d = _double;
    }
    return *endptr == '\0';
}

int main(int argc, char *argv[])
{
    printf("Arguments: %i\n", argc);
    for(int i = 0; i < argc; i++) {
        const char *type;
        if (cstr_to_long(argv[i], NULL)) {
            type = "long";
        } else if (cstr_to_double(argv[i], NULL)) {
            type = "double";
        } else {
            type = "string";
        }
        printf(" * %2i: '%s' (%s)\n", i, argv[i], type);
    }
}
