#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "base64.h"

int main(int argc, char *argv[])
{
    int exit_code = 0;
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
    int out_len = base64_decode(in, NULL, 0);
    if (out_len >= 0) {
        char *out = malloc(out_len + 1);
        // Decode base64
        int len = base64_decode(in, out, out_len + 1);
        if (len >= 0 && len <= out_len) {
            out[len] = '\0';
            // Write out to standard out
            fwrite(out, 1, len, stdout);
        } else {
            exit_code = -1;
        }
        free(out);
    } else {
        exit_code = -1;
    }
    // Report error, if needed
    if (exit_code != 0) {
        fprintf(stderr, "%s: Invalid input!\n", argv[0]);
    }
    // Clean-up 
    free(in);
    return exit_code;
}
