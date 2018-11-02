/*
 * Step 4: Put Argument Related Stuff to a Struct
 *
 * Topics:
 *  * Use typedef directely with the definition
 *  * Create struct and new type with typedef
 *
 * To compile and link this program call:
 * 
 * $ gcc -o main main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef enum {
    ARG_TYPE_CSTR,
    ARG_TYPE_LONG,
    ARG_TYPE_DOUBLE,
    ARG_TYPE_INVALID,
} ArgType;

typedef union {
    const char *cstr;
    long l;
    double d;
} ArgValue;

typedef struct {
    int position;
    ArgType type;
    ArgValue value;
} Arg;


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

static Arg arg_init(int pos, const char *arg_cstr)
{
    Arg arg;
    arg.position = pos;
    if (cstr_to_long(arg_cstr, &arg.value.l)) {
        arg.type = ARG_TYPE_LONG;
    } else if (cstr_to_double(arg_cstr, &arg.value.d)) {
        arg.type = ARG_TYPE_DOUBLE;
    } else {
        arg.value.cstr = arg_cstr;
        arg.type = ARG_TYPE_CSTR;
    }
    return arg;
}

static const char *arg_type_to_cstr(Arg *arg)
{
    return argtype2cstr[arg->type];
}

static void arg_print(Arg *arg)
{
    printf(" * %2i: '", arg->position);
    switch (arg->type) {
    case ARG_TYPE_CSTR:
        printf("%s", arg->value.cstr);
        break;
    case ARG_TYPE_LONG:
        printf("%li", arg->value.l);
        break;
    case ARG_TYPE_DOUBLE:
        printf("%g", arg->value.d);
        break;
    default:
        printf("invalid");
        break;
    }
    printf("' (%s)\n", arg_type_to_cstr(arg));
}

int main(int argc, char *argv[])
{
    printf("Arguments: %i\n", argc);
    for(int i = 0; i < argc; i++) {
        Arg arg = arg_init(i, argv[i]);
        arg_print(&arg);
    }
}
