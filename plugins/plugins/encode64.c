#include <string.h>
#include <stdint.h>

#include "../plugins.h"


static const char BASE64_TABLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz0123456789+/";
static const char BASE64_PADDING = '=';


int encode64(const char *src, char *dest, size_t dest_size)
{
    size_t src_len = strlen(src);
    size_t dest_pos = 0;
    uint8_t in[3];
    uint8_t out[4];
    int dest_len = (src_len / 3 + 1) * 4;
    // Check destination arguments
    if (dest == NULL || dest_size == 0) {
        return dest_len;
    }
    // Process the first bytes without worring about padding
    while (src_len > 2) {
        in[0] = *src++;
        in[1] = *src++;
        in[2] = *src++;
        src_len -= 3;
        out[0] = in[0] >> 2;
        out[1] = ((in[0] & 0x03) << 4) + (in[1] >> 4);
        out[2] = ((in[1] & 0x0f) << 2) + (in[2] >> 6);
        out[3] = in[2] & 0x3f;
        // Check space in the destination C-string
        if (dest_pos + 4 > dest_size) {
            return dest_len;
        }
        // Copy to destination buffer
        dest[dest_pos++] = BASE64_TABLE[out[0]];
        dest[dest_pos++] = BASE64_TABLE[out[1]];
        dest[dest_pos++] = BASE64_TABLE[out[2]];
        dest[dest_pos++] = BASE64_TABLE[out[3]];
    }
    // Process the remaining bytes with paddind in mind
    if (src_len > 0) {
        // Get what is left
        in[0] = in[1] = in[2] = '\0';
        for (size_t i = 0; i < src_len; i++) {
            in[i] = *src++;
        }
        out[0] = in[0] >> 2;
        out[1] = ((in[0] & 0x03) << 4) + (in[1] >> 4);
        out[2] = ((in[1] & 0x0f) << 2) + (in[2] >> 6);
        // Check space in the destination C-string
        if (dest_pos + 4 > dest_size) {
            return -1;
        }
        // Copy to destination buffer
        dest[dest_pos++] = BASE64_TABLE[out[0]];
        dest[dest_pos++] = BASE64_TABLE[out[1]];
        if (src_len == 1) {
            dest[dest_pos++] = BASE64_PADDING;
        } else {
            dest[dest_pos++] = BASE64_TABLE[out[2]];
        }
        dest[dest_pos++] = BASE64_PADDING;
    }
    if (dest_pos >= dest_size) {
        return -1;
    }
    // Terminate destination C-string
    dest[dest_pos] = '\0';
    return dest_pos;
}

bool init_encode64(Plugin *plugin)
{
    plugin->filter_cb = encode64;
    return true;
}
