/**
 * This is an idea of extending the printf function.
 *
 * https://stackoverflow.com/questions/9260170/is-this-possible-to-customize-printf
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


typedef struct {
    int x;
    int y;
} Point;


long max(long a, long b)
{
    return a > b ? a : b;
}

size_t vformat(char *cstr, size_t size, char const *fmt, va_list va)
{
    size_t len = 0;
    char const *p = fmt, *end = p + strlen(fmt), *fmt_p = fmt, *fmt_q = fmt;
    va_list ap_p, ap_q;
    va_copy(ap_p, va);
    va_copy(ap_q, va);
    // Parse the fmt string by looping over each singel char
    do {
        // Check for literal char
        if ((*p != '%') || (*++p == '%')) {
            fmt_q = p + 1;
            continue;
        }
        // Check for parameter field
        char *q;
        strtoul(p, &q, 10);
        if ((q != p) && (*q == '$')) {
            p = q + 1;
        }
        // Check for flags
        bool flags_quit = false;
        do {
            switch (*p) {
            case '-':
            case '+':
            case ' ':
            case '0':
            case '#':
                p++;
                break;
            default:
                flags_quit = true;
                break;
            }
        } while (!flags_quit || p >= end);
        // Check for width field
        if (*p == '*') {
            va_arg(ap_q, int);
            p++;
        } else {
            strtoul(p, &q, 10);
            p = q;
        }
        // Check for precision field
        if (*p == '.') {
            p++;
            strtoul(p, &q, 10);
            p = q;
        }
        // Length modifiers
        char length[2] = { '\0', '\0' };
        switch (*p) {
        case 'h':
        case 'l':
            length[0] = *p++;
            if ((*p == 'h') || (*p == 'l')) length[1] = *p++;
            break;
        case 'L':
        case 'z':
        case 'j':
        case 't':
            length[0] = *p++;
            break;
        }
        // Types
        switch (*p) {
        // int or unsigned int
        case 'i':
        case 'd':
        case 'u':
        case 'x':
        case 'X':
        case 'o':
            switch (length[0]) {
            case '\0':
                // no length modifier
                if ((*p == 'i') || (*p == 'd')) {
                    // int
                    va_arg(ap_q, int);
                } else {
                    // unsigned int
                    va_arg(ap_q, unsigned int);
                }
            case 'h':
                if (length[1] == 'h') {
                    // char (promoted to int)
                    va_arg(ap_q, int);
                } else {
                    // short (promoted to int)
                    va_arg(ap_q, int);
                }
                break;
            case 'L':
                if ((*p == 'i') || (*p == 'd')) {
                    if (length[1] == 'L') {
                        // long
                        va_arg(ap_q, long);
                    } else {
                        // long long
                        va_arg(ap_q, long long);
                    }
                } else {
                    if (length[1] == 'L') {
                        // unsigned long
                        va_arg(ap_q, unsigned long);
                    } else {
                        // unsigned long long
                        va_arg(ap_q, unsigned long long);
                    }
                }
                break;
            case 'z':
                // size_t
                va_arg(ap_q, size_t);
                break;
            case 'j':
                // intmax_t
                va_arg(ap_q, intmax_t);
                break;
            case 't':
                // ptrdiff_t
                va_arg(ap_q, ptrdiff_t);
                break;
            }
            break;
        // double
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        case 'a':
        case 'A':
            switch (length[0]) {
            case 'L':
                // long double
                va_arg(ap_q, long double);
                break;
            case 'l':
                // Does nothing
            default:
                // double
                va_arg(ap_q, double);
            }
            break;
        case 's':
            // char *
            va_arg(ap_q, char *);
            break;
        case 'c':
            // char (promoted to int)
            va_arg(ap_q, int);
            break;
        case 'p':
            // void *
            va_arg(ap_q, void *);
            break;
            // int *
        case 'n':
            va_arg(ap_q, int *);
            break;
        // Custom type
        case 'P': {
            Point point = va_arg(ap_q, Point);
            if (fmt_q != fmt_p) {
                char *sub_fmt = strndup(fmt_p, fmt_q - fmt_p);
                len += vsnprintf(cstr + len, max(0, size - len), sub_fmt, ap_p);
                free(sub_fmt);
                // 'ap_p' can only be used once, end it.
                va_end(ap_p);
                // Initialise 'ap_p' with the next vas
                va_copy(ap_p, ap_q);
            }
            len += snprintf(cstr + len, max(0, size - len), "(%i, %i)",
                    point.x, point.y);
            fmt_p = p + 1;
            break;
        }
        default:
            break;
        }
        fmt_q = p + 1;
    } while (++p < end);
    //Print out the rest of the format string.
    if (*fmt_p) {
        len += vsnprintf(cstr + len, max(0, size - len), fmt_p, ap_p);
    }
    // Cleanup
    va_end(ap_p);
    va_end(ap_q);
    return len;
}

size_t format(char *cstr, size_t size, char const *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    size_t len = vformat(cstr, size, fmt, va);
    va_end(va);
    return len;
}


int main(int argc, char *argv[])
{
    char cstr[128];
    Point p = { .x = 12, .y = 34 };
    size_t len = format(cstr, sizeof(cstr), "Point: %P, val: %i.\n", p, 42);
    fwrite(cstr, 1, len, stdout);
    printf("len: %zu\n", len);
    return 0;
}
