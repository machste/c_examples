#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "base64.h"

static const char BASE64_TABLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz0123456789+/";
static const char BASE64_PADDING = '=';

int base64_index(char ch)
{
    for (int i = 0; i < sizeof(BASE64_TABLE); i++) {
        if (ch == BASE64_TABLE[i]) {
            return i;
        }
    }
    return -1;
}

int base64_encode(const void *_src, size_t src_len, char *dest,
        size_t dest_size)
{
    const uint8_t *src = _src;
    size_t dest_pos = 0;
    uint8_t in[3];
    uint8_t out[4];
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
            return -1;
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

int base64_decode(const char *src, void *_dest, size_t dest_size)
{
    uint8_t *dest = _dest; 
    size_t dest_pos = 0;
    char ch;
    int position = 0;
    uint8_t next_dest_byte;
    // Process base64 characters from src C-string
    while ((ch = *src++) != '\0') {
        // Skip whitespace anywhere
        if (isspace(ch)) {
            continue; 
        }
        // End loop on padding character
        if (ch == BASE64_PADDING) {
            break;
        }
        // Lookup base64 index and exit on error
        int base_idx = base64_index(ch);
        if (base_idx < 0) {
            return -1;
        }
        // Process character depending on its position
        if (position == 0) {
            if (dest != NULL) {
                if (dest_pos >= dest_size) {
                    return -1;
                }
                dest[dest_pos] = base_idx << 2;
            }
            position = 1;
        } else if (position == 1) {
            next_dest_byte = (base_idx & 0x0f) << 4;
            if (dest != NULL) {
                dest[dest_pos] |= base_idx >> 4;
                if (dest_pos + 1 < dest_size) {
                    dest[dest_pos + 1] = next_dest_byte;
                } else if (next_dest_byte != 0) {
                    return -1;
                }
            }
            dest_pos++;
            position = 2;
        } else if (position == 2) {
            next_dest_byte = (base_idx & 0x03) << 6;
            if (dest != NULL) {
                dest[dest_pos] |= base_idx >> 2;
                if (dest_pos + 1 < dest_size) {
                    dest[dest_pos + 1] = next_dest_byte;
                } else if (next_dest_byte != 0) {
                    return -1;
                }
            }
            dest_pos++;
            position = 3;
        } else if (position == 3) {
            if (dest != NULL) {
                if (dest_pos >= dest_size) {
                    return -1;
                }
                dest[dest_pos] |= base_idx;
            }
            dest_pos++;
            position = 0;
        } else {
            abort();
        }
    }
    // We are done decoding base64 characters, check the padding characters
    if (ch == BASE64_PADDING) {
        // We got a padding character, thus position 0 and 1 are invalid
        if (position == 0 || position == 1) {
            return -1;
        }
        // Skip it and get next character
        ch = *src++;
        switch (position) {
        case 2:
            // Skip all spaces
            for (; ch != '\0'; ch = *src++) {
                if (!isspace(ch)) {
                    break;
                }
            }
            // We are on position 2, thus check for another padding character
            if (ch != BASE64_PADDING) {
                return -1;
            }
            // Skip it and get next character
            ch = *src++;
            // Fall through to "single padding" case.
        case 3:
            // Only spaces or the terminating null are allowed here, skip them 
            for (; ch != '\0'; ch = *src++) {
                if (!isspace(ch)) {
                    return -1;
                }
            }
            /*
             * Now make sure for cases 2 and 3 that the "extra" bits that
             * slopped past the last full byte were zeros. If we do not check
             * them, they become a subliminal channel.
             */
            if (next_dest_byte != 0) {
                return -1;
            }
        }
    } else {
        // We reached end of string, thus we have to be on position 0
        if (position != 0) {
            return -1;
        }
    }
    // Null-terminate if we have room left
    if (dest_pos < dest_size) {
        dest[dest_pos] = 0;
    }
    return dest_pos;
}
