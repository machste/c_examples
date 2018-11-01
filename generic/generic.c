#include <unistd.h>

#define write(a, b, c) _Generic((a), Io *: __write__, default: write)(a, b, c)

typedef struct {
    int fd;
} Io;

ssize_t __write__(Io *self, const void *data, size_t size)
{
    write(self->fd, data, size);
}

void main() {
    const char text[] = "Hallo Welt!\n";
    Io io = { .fd = STDOUT_FILENO };
    write(&io, text, sizeof(text));
    write(STDOUT_FILENO, text, sizeof(text));
}
