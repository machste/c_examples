/*
 * Step 4: Union for the Values of the Different Argument Types
 *
 * Topics:
 *  * Create union and new type with typedef
 *  * Switch case statement
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

union _ArgValue {
    const char *cstr;
    long l;
    double d;
};

typedef enum _ArgType ArgType;
typedef union _ArgValue ArgValue;


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

static void arg_print_value(ArgType type, ArgValue value)
{
    switch (type) {
    case ARG_TYPE_CSTR:
        printf("%s", value.cstr);
        break;
    case ARG_TYPE_LONG:
        printf("%li", value.l);
        break;
    case ARG_TYPE_DOUBLE:
        printf("%g", value.d);
        break;
    default:
        printf("invalid");
        break;
    }
}

int main(int argc, char *argv[])
{
    printf("Arguments: %i\n", argc);
    for(int i = 0; i < argc; i++) {
        ArgType type;
        ArgValue value;
        if (cstr_to_long(argv[i], &value.l)) {
            type = ARG_TYPE_LONG;
        } else if (cstr_to_double(argv[i], &value.d)) {
            type = ARG_TYPE_DOUBLE;
        } else {
            value.cstr = argv[i];
            type = ARG_TYPE_CSTR;
        }
        printf(" * %2i: '", i);
        arg_print_value(type, value);
        printf("' (%s)\n", arg_type_to_cstr(type));
    }
}
