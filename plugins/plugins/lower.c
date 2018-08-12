#include <string.h>
#include <ctype.h>

#include "../plugins.h"


int lower(const char *src, char *dest, size_t dest_size)
{
    int src_len = strlen(src);
    if (dest != NULL && dest_size > 0) {
        size_t i;
        for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
            dest[i] = tolower(src[i]);
        }
        dest[i] = '\0';
    }
    return src_len;
}

bool init_lower(Plugin *plugin)
{
    plugin->filter_cb = lower;
    return true;
}
