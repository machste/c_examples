#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "base64.h"

int main(int argc, char *argv[])
{
    // Analyse arguments
    size_t in_size = 0;
    size_t argv_len[argc];
    for (int i = 1; i < argc; i++) {
        size_t arg_len = strlen(argv[i]);
        argv_len[i] = arg_len;
        // Add argument length puls one for either a space or null-termination
        in_size += (arg_len + 1);
    }
    // Concatinate all in arguments
    size_t in_pos = 0;
    char *in = malloc(in_size);
    for (int i = 1; i < argc; i++) {
        memcpy(in + in_pos, argv[i], argv_len[i]);
        in_pos += argv_len[i];
        // Append space between arguments
        if (i < argc - 1) in[in_pos++] = ' ';
    }
    in[in_pos] = '\0';
    // Prepare Output
    size_t out_size = (in_pos / 3 + 1) * 4 + 1;
    char *out = malloc(out_size);
    // Encode base64
    if (base64_encode(in, in_pos, out, out_size) > 0) {
        // Write out to standard out
        puts(out);
    } else {
        exit(-1);
    }
    // Clean-up 
    free(in);
    free(out);
}
