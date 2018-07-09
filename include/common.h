#ifndef _DEFS_H_
#define _DEFS_H_

#include <stdlib.h>
#include <stdint.h>

#include "settings.h"

#ifndef NULL
#define NULL	(void *)0
#endif

#define FALSE	(0)
#define TRUE	(!FALSE)

#define NELEMS(a)	((int)(sizeof(a) / sizeof((a)[0])))

#include <stdio.h>


#ifdef DEBUG
	#define dprintf(...)	printf(__VA_ARGS__)
#else
	#define dprintf(...)
	#define printf(...)
#endif

#define todo()	do {								\
			fprintf(stderr, "\nerror: %s function"			\
				" unfinished in file %s at line %d\n\n",	\
				__FILE__, __FUNCTION__, __LINE__);		\
			exit(1);						\
		} while(0)


typedef int8_t sbyte;
typedef uint8_t byte;
typedef uint16_t word;

void die(const char *msg);


#endif /* _DEFS_H_ */
