#ifndef _ARG_H_
#define _ARG_H_

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


Arg arg_init(int pos, const char *arg_cstr);

const char *arg_type_to_cstr(Arg *arg);

void arg_print(Arg *arg);


#endif /* _ARG_H_ */
