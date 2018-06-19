#ifndef _MAPPER_DEFS_H_
#define _MAPPER_DEFS_H_

#include "common.h"


struct mapper {
	void (*init)(void);
	byte (*getb)(word);
	void (*setb)(word, byte);
};

extern struct mapper map[256];
extern byte mapper;


#endif /* _MAPPER_DEFS_H_ */
