#ifndef _BASE64_H_
#define _BASE64_H_

#include <stddef.h>

int base64_encode(const void *src, size_t src_len, char *dest,
        size_t dest_size);

int base64_decode(const char *src, void *dest, size_t dest_size);

#endif /* _BASE64_H_ */
