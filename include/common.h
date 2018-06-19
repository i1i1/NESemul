#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdint.h>

#ifndef NULL
#define NULL	(void *)0
#endif

#define FALSE	(0)
#define TRUE	(!FALSE)

#define NELEMS(a)	(sizeof(a) / sizeof(a[0]))

#ifdef DEBUG
	#include <stdio.h>
	#define dprintf(...)	printf(__VA_ARGS__)
#else
	#define dprintf(...)
	#define printf(...)
#endif


typedef int8_t sbyte;
typedef uint8_t byte;
typedef uint16_t word;

byte inb();
void outb(byte v);
word inw();
void outw(word v);
void die(const char *msg);


#endif /* _DEFS_H_ */
