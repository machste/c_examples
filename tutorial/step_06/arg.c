#include <stdio.h>

#include "math.h"
#include "arg.h"


static char *argtype2cstr[] = {
    [ARG_TYPE_CSTR] = "string",
    [ARG_TYPE_LONG] = "long",
    [ARG_TYPE_DOUBLE] = "double",
    [ARG_TYPE_INVALID] = "invalid",
};


Arg arg_init(int pos, const char *arg_cstr)
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

const char *arg_type_to_cstr(Arg *arg)
{
    return argtype2cstr[arg->type];
}

void arg_print(Arg *arg)
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
