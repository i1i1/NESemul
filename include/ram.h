#ifndef _RAM_H_
#define _RAM_H_

#include "common.h"

struct prg_rom {
	byte arr[16384];
};

extern struct prg_rom **prg;
extern byte prg_max;

#endif /* _RAM_H_ */
