/*
 * Plugins in C
 *
 * This example has been inspired by Eli Bendersky's article [1]
 * published in his blog, see http://eli.thegreenplace.net.
 *
 * [1] https://eli.thegreenplace.net/2012/08/24/plugins-in-c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "convert.h"


static Plugins *plugins = NULL;


Plugins *convert_get_plugins(void) {
    return plugins;
}

int main(int argc, char *argv[])
{
    int ret = EXIT_SUCCESS;
    // Discover all plugins in the plugins folder
    plugins = plugins_new();
    if (!plugins_discover(plugins, "./plugins")) {
        ret = EXIT_FAILURE;
    } 
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
    printf("Input: %s\n", in);
    if (plugins->count == 0) {
        fprintf(stderr, "No plugins found!\n");
        ret = EXIT_FAILURE;
    }
    // Apply all filters
    for (int i = 0; i < plugins->count; i++) {
        Plugin *plugin = plugins_get_index(plugins, i);
        // Check plugin for a valid filter callback
        if (plugin->filter_cb == NULL) {
            fprintf(stderr, "No filter for '%s' defined!\n", plugin->name);
            ret = EXIT_FAILURE;
            continue;
        }
        // Prepare Output
        int out_size = plugin->filter_cb(in, NULL, 0) + 1;
        if (out_size > 0) {
            char *out = malloc(out_size);
            plugin->filter_cb(in, out, out_size);
            // Write out to standard out
            printf("Output(%s): %s\n", plugin->name, out);
            free(out);
        } else {
            fprintf(stderr, "Filter '%s' failed!\n", plugin->name);
            ret = EXIT_FAILURE;
        }
    }
    // Clean-up
    free(in);
    plugins_free(plugins);
    return ret;
}
