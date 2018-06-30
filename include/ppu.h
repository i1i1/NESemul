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
	byte ready;

	byte ram[0x4000];
	byte spr_ram[0x100];

	byte PPUCTRL;
	byte PPUMASK;
	byte PPUSTATUS;
	byte OAMADDR;
	byte PPUSCROLL_X;
	byte PPUSCROLL_Y;
	word PPUADDR;

	byte scroll;
	byte addr;
	byte vmap;

	int scanline;
};

extern struct ppu ppu;
extern const byte ppu_palette[3 * 0x40];

void ppu_init();

byte ppu_reg_get(word addr);
void ppu_reg_set(word addr, byte b);

byte ppu_is_reg_r(word addr);

byte ppu_getb(word a);
void ppu_setb(word a, byte b);

void ppu_run_cycles(int n);

#endif /* _PPU_H_ */
