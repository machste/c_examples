#include <stdio.h>
#include <malloc.h>


int main (int argc, char **argv)
{
    void *ptr = NULL;
    for (int i = 1; i < 16; i++) {
        ptr = realloc(ptr, i);
        printf("ptr: %p, size: %i, allocated: %zu\n", ptr,
                i, malloc_usable_size(ptr));
    }
    malloc_info(0, stdout);
    free(ptr);
    return 0;
}
