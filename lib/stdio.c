#include "include/libc.h"
#include "hal.h"
#include "type.h"


void printf(const char *fmt, ...)
{
    va_list vargs;
    va_start(vargs, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
            case '\0': {
                __putchar('%');
                goto end;
            }
            case '%': {
                __putchar('%');
                break;
            }
            // String Type
            case 's': {
                const char *s = va_arg(vargs, const char *);
                while (*s) {
                    __putchar(*s);
                    s++;
                }
                break;
            }
            // Decimal Type
            case 'd': {
                int value = va_arg(vargs, int);
                if (value == 0) {
                    __putchar('0');
                    break;
                }
                if (value < 0) {
                    __putchar('-');
                    value = -value;
                }

                char buf[10];
                int i = 0;
                while (value > 0) {
                    buf[i++] = '0' + value % 10;
                    value /= 10;
                }
                while (i--)
                    __putchar(buf[i]);
                break;
            }
            case 'x': {
                unsigned value = va_arg(vargs, unsigned);
                printf("0x");
                for (int i = 7; i >= 0; i--) {
                    unsigned nibble = (value >> (i * 4)) & 0xF;
                    __putchar("0123456789ABCDEF"[nibble]);
                }
            }
            }

        } else {
            __putchar(*fmt);
        }
        fmt++;
    }
end:
    va_end(vargs);
}
