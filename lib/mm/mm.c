#include "mm.h"

void *memset( void *buf, char c, size_t n){
	uint8_t *p = (uint8_t *) buf;
	while( n--)
		*p++ = c;
	return buf;
}

void *memcpy(char *dst, const char *src, size_t n){
	uint8_t *d = (uint8_t *)dst;
	const uint8_t *s = (const uint8_t *)src;
	while(n--){
		*d++ = *s++;
	}
	return dst;
}