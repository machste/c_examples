#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "base64.h"

int main(int argc, char *argv[])
{
    size_t in_len = 0;
    size_t argv_len[argc];
    // Analyse arguments
    for (int i = 1; i < argc; i++) {
        size_t arg_len = strlen(argv[i]);
        argv_len[i] = arg_len;
        in_len += arg_len;
    }
    // Concatinate all in arguments
    size_t in_pos = 0;
    char *in = malloc(in_len + argc);
    for (int i = 1; i < argc; i++) {
        memcpy(in + in_pos, argv[i], argv_len[i]);
        in_pos += argv_len[i];
        // Append space between arguments
        if (i < argc - 1) in[in_pos++] = ' ';
    }
    in[in_pos] = '\0';
    // Prepare Output
    size_t out_size = (in_len / 3 + 1) * 4 + 1;
    char *out = malloc(out_size);
    // Encode base64
    if (base64_encode(in, in_len, out, out_size) > 0) {
        // Write out to standard out
        puts(out);
    } else {
        exit(-1);
    }
    // Clean-up 
    free(in);
    free(out);
}
