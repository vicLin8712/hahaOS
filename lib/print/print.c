#include "sbi/sbi.h"
#include "print/print.h"
#include "type.h"

void putchar(char ch) {
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */);
}

void print(const char *fmt, ...){
    va_list  vargs;
    va_start(vargs, fmt);
    
    while(*fmt){ 
        if(*fmt == '%'){
	       fmt++;
		   switch ( *fmt){
			    case '\0':{
                    putchar( '%');
				    goto end;
				}
			    case '%':{
				    putchar('%');
					break;
				}
				// String Type
				case 's':{
				    const char *s = va_arg(vargs, const char *);
					while (*s){
						putchar(*s);
						s++;
					}
					break;
				}
			    // Decimal Type
				case 'd':{
				    int value = va_arg(vargs, int);
					if (value == 0){
						putchar('0');
						break;
					}
					if (value < 0){
						putchar ('-');
					    value = - value;
					}

					char buf [10];
					int i = 0;
					while (value > 0){
						buf [i++] = '0' + value%10 ;
						value /= 10;
					}
					while (i--)
					    putchar(buf[i]);
					break;
				}
		   }

		}
		else{
		    putchar(*fmt);
		}
		fmt++;

    }
	end:
	    va_end(vargs);
}
