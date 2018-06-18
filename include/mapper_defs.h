#ifndef _MAPPER_DEFS_H_
#define _MAPPER_DEFS_H_

#include "common.h"


struct mapper {
	void (*init)();
	byte (*inb)();
	void (*outb)(byte);
};

extern struct mapper map[256];


#endif /* _MAPPER_DEFS_H_ */
