/*
 * Step 7: Use Makefile
 *
 * Topics:
 *  * Create a Makefile
 *  * Define compiler flags
 *  * Use an array for all arguments
 *
 * To compile and link this program call:
 *
 * $ make
 *
 * To test the program run
 * $ ./main 12 34.2 42 gdf 34 -0.34 0xff gaga
 */

#include <stdio.h>
#include <stdbool.h>

#include "arg.h"


#define ARRAY_LEN(x) (sizeof(x) / sizeof((x)[0]))


static void add_args_of_type(Arg *args, int args_len, ArgType type)
{
    ArgValue sum;
    // Init sum
    if (type == ARG_TYPE_LONG) {
        sum.l = 0;
    } else if (type == ARG_TYPE_DOUBLE) {
        sum.d = 0;
    }
    // Iterate over arguments and sum them up
    bool first = true;
    for (int i = 0; i < args_len; i++) {
        if (args[i].type == type) {
            if (type == ARG_TYPE_LONG) {
                sum.l += args[i].value.l;
            } else if (type == ARG_TYPE_DOUBLE) {
                sum.d += args[i].value.d;
            }
            if (first) {
                printf(" * %s: ", arg_type_to_cstr(&args[i]));
                first = false;
            } else {
                switch (type) {
                case ARG_TYPE_LONG:
                case ARG_TYPE_DOUBLE:
                    printf(" + ");
                    break;
                default:
                    printf(", ");
                    break;
                }
            }
            arg_value_print(&args[i]);
        }
    }
    if (!first) {
        // Print sum
        if (type == ARG_TYPE_LONG) {
            printf(" = %li\n", sum.l);
        } else if (type == ARG_TYPE_DOUBLE) {
            printf(" = %g\n", sum.d);
        } else {
            printf("\n");
        }
    }
}

int main(int argc, char *argv[])
{
    printf("Arguments: %i\n", argc);
    Arg args[argc];
    for(int i = 0; i < argc; i++) {
        args[i] = arg_init(i, argv[i]);
        arg_print(&args[i]);
    }
    printf("Sort by argument type:\n");
    for(int type = 0; type < ARG_TYPE_INVALID; type++) {
        add_args_of_type(args, ARRAY_LEN(args), type);
    }
}
