#include "mm.h"

void *memset( void *buf, char c, size_t n){
	uint8_t *p = (uint8_t *) buf;
	while( n--)
		*p++ = c;
	return buf;
}

char *memcpy(char *dst, const char *src, size_t n){
	uint8_t *d = (uint8_t *)dst;
	const uint8_t *s = (const uint8_t *)src;
	while(n--){
		*d++ = *s++;
	}
	return dst;
}

int memcmp(const char *s1, const char *s2){
	while (*s1 && *s2){
		if (*s1 != *s2)
			break;
		s1 ++;
		s2 ++;
	}
	return *(unsigned char *)s1 - *(unsigned char *)s2;
}