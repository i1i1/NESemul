#ifndef _RAM_H_
#define _RAM_H_

#include "common.h"

#define PRG_ROM_BANK_SZ		(16 * 1024)

struct prg_rom {
	/* Array of arrays of banks of PRG_ROM_BANK_SZ */
	byte **bank;
	byte n;

	/* Mapper info */
	byte low;
	byte up;
};

extern struct prg_rom prg_rom;

/* Functions for emulator usage */
byte ram_getb(word addr);
word ram_getw(word addr);
void ram_setb(word addr, byte b);

/* Functions for mappers usage */
byte ram_general_getb(word addr);
void ram_general_setb(word addr, byte b);

void ram_init();

#endif /* _RAM_H_ */
