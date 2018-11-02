/*
 * Step 3: Argument Type as Enumeration
 *
 * Topics:
 *  * Create an enumeration for the differnet argument types
 *  * Create an argument type with typedef
 *  * Use of local variables
 *
 * To compile and link this program call:
 * 
 * $ gcc -o main main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


enum _ArgType {
    ARG_TYPE_CSTR,
    ARG_TYPE_LONG,
    ARG_TYPE_DOUBLE,
    ARG_TYPE_INVALID,
};

typedef enum _ArgType ArgType;


static char *argtype2cstr[] = {
    [ARG_TYPE_CSTR] = "string",
    [ARG_TYPE_LONG] = "long",
    [ARG_TYPE_DOUBLE] = "double",
    [ARG_TYPE_INVALID] = "invalid",
};


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

static const char *arg_type_to_cstr(ArgType type)
{
    return argtype2cstr[type];
}

int main(int argc, char *argv[])
{
    printf("Arguments: %i\n", argc);
    for(int i = 0; i < argc; i++) {
        ArgType type;
        if (cstr_to_long(argv[i], NULL)) {
            type = ARG_TYPE_LONG;
        } else if (cstr_to_double(argv[i], NULL)) {
            type = ARG_TYPE_DOUBLE;
        } else {
            type = ARG_TYPE_CSTR;
        }
        printf(" * %2i: '%s' (%s)\n", i, argv[i], arg_type_to_cstr(type));
    }
}
