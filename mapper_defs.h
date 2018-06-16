#ifndef _MAPPER_DEFS_H_
#define _MAPPER_DEFS_H_

#include "defs.h"


struct mapper {
	byte (*inb)();
	void (*outb)(byte);
};


#endif /* _MAPPER_DEFS_H_ */
