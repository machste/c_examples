#include <stdlib.h>

#include "math.h"


bool cstr_to_long(const char *cstr, long *l)
{
    char *endptr;
    long _long = strtol(cstr, &endptr, 0);
    if (l != NULL) {
        *l = _long;
    }
    return *endptr == '\0';
}

bool cstr_to_double(const char *cstr, double *d)
{
    char *endptr;
    double _double = strtod(cstr, &endptr);
    if (d != NULL) {
        *d = _double;
    }
    return *endptr == '\0';
}
