#include <stdio.h>

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Circle {
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

void main() {
    Circle c = {.x = 1, .y = 2, .r = 3};
    point_print(&c);
    circle_print(&c);
}
