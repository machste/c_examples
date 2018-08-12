#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../convert.h"
#include "../plugins.h"

int title(const char *src, char *dest, size_t dest_size)
{
    Plugins *plugins = convert_get_plugins();
    Plugin *upper_plugin = plugins_get_by_name(plugins, "upper");
    if (upper_plugin == NULL) {
        fprintf(stderr, "Plugin 'title' depends on 'upper'!\n");
        return -1;
    }
    int src_len = strlen(src);
    if (dest != NULL && dest_size > 0) {
        size_t intm_size = dest_size;
        char *intm = malloc(intm_size);
        size_t i;
        size_t j = 0;
        for (i = 0; i < intm_size - 1 && src[i] != '\0'; i++) {
            intm[j++] = src[i];
            if (i < src_len - 1) {
                intm[j++] = ' ';
            } else {
                intm[j] = '\0';
            }
        }
        upper_plugin->filter_cb(intm, dest, dest_size);
        free(intm);
    }
    return src_len * 2 - 1;
}

bool init_title(Plugin *plugin)
{
    plugin->filter_cb = title;
    return true;
}
