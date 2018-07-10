#include <stdio.h>

#include "shared.h"
 

int main(void)
{
    puts("This is a shared library program.");
    foo();
    return 0;
}
