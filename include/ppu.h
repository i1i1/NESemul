#ifndef _PPU_H_
#define _PPU_H_

#include "common.h"

enum ppu_reg {
	PPUCTRL		= 0x2000,
	PPUMASK		= 0x2001,
	PPUSTATUS	= 0x2002,
	OAMADDR		= 0x2003,
	OAMDATA		= 0x2004,
	PPUSCROLL	= 0x2005,
	PPUADDR 	= 0x2006,
	PPUDATA 	= 0x2007,
	OAMDMA		= 0x4014,
};

struct ppu {
	byte ram[0x4000];

	byte PPUCTRL;
	byte PPUMASK;
	byte PPUSTATUS;
	byte OAMADDR;
	byte OAMDATA;
	byte PPUSCROLL;
	byte PPUADDR;
	byte PPUDATA;
	byte OAMDMA;
};

extern struct ppu ppu;

void ppu_init();

byte ppu_getb();
void ppu_setb(word addr, byte b);

#endif /* _PPU_H_ */
