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

struct chr_rom {
	/* Array of arrays of banks of PRG_ROM_BANK_SZ */
	byte **bank;
	byte n;

	/* Mapper info */
	byte cur;
};

extern struct prg_rom prg_rom;
extern struct chr_rom chr_rom;

/* Functions for emulator usage */
byte ram_getb(word addr);
word ram_getw(word addr);
void ram_setb(word addr, byte b);

/* Functions for mappers usage */
byte ram_general_getb(word addr);
void ram_general_setb(word addr, byte b);

/* Some stack rutines */
void ram_pushb(byte b);
void ram_pushw(word w);
byte ram_popb();
word ram_popw();

void ram_init();

#endif /* _RAM_H_ */
