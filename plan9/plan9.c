#include <stdio.h>
#include <stdint.h>

/*** First Example ***/

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point;
    int r;
} Circle;

void point_print(Point *self)
{
    printf("Point: x: %i, y: %i\n", self->x, self->y);
}

void circle_print(Circle *self)
{
    printf("Circle: x: %i, y: %i, r: %i\n", self->x, self->y, self->r);
}

/*** Second Example ***/

typedef struct {
    uint16_t a1;
    uint16_t a2;
} A;

typedef struct {
    uint8_t b1[sizeof(uint16_t)];
    uint8_t b2[sizeof(uint16_t)];
} B;

typedef struct {
    union {
        A;
        B;
    };
    char *data;
} AB;

void a_print(A *self)
{
    printf("A: a1: 0x%04x, a2: 0x%04x\n", self->a1, self->a2);
}

void b_print(B *self)
{
    printf("B: b1: %02x %02x, b2: %02x %02x\n", self->b1[0], self->b1[1],
            self->b2[0], self->b2[1]);
}

void ab_print(AB *self)
{
    printf("AB: a1: 0x%04x, b2: %02x %02x, data: %s\n", self->a1, self->b2[0],
            self->b2[1], self->data);
}


/*** Test Program ***/

void main() {
    // First example
    printf("First example\n");
    Circle c = { .x = 1, .y = 2, .r = 3 };
    point_print(&c);
    circle_print(&c);
    // Second example
    printf("\nSecond example\n");
    AB ab = { .a1 = 0x1234, .a2 = 0xC0DE, .data = "Hallo Welt!" };
    a_print(&ab);
    b_print(&ab);
    ab_print(&ab);
}
