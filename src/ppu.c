#include "common.h"
#include "ram.h"
#include "ppu.h"

struct ppu ppu;


byte
ppu_reg_get(word addr)
{
	/* If some WO register */
	if (addr >= 0x4000)
		return 0;
	/* Some mirroring */
	else
		addr = 0x2000 + (addr - 0x2000) % 8;

	printf("Accessing %04x register\n", addr);

	switch (addr) {
	case PPUSTATUS:
		return 0xff;
//		return ppu.PPUSTATUS;
	case PPUDATA:
		return ppu.PPUDATA;
	/* If some WO register or invalide register */
	default:
		return 0;
	}
}

void
ppu_load_spr_ram(word addr)
{
	int i;

	for (i = 0; i < 0x100; i++)
		ppu.spr_ram[i] = ram_getb(addr + i);
}

void
ppu_reg_set(word addr, byte b)
{
	/* Some mirroring */
	if (addr < 0x4000)
		addr = 0x2000 + addr % 8;

	switch (addr) {
	case OAMDMA:
		ppu_load_spr_ram(b * 0x100);
		break;
	default:
		todo();
	}
}

byte
ppu_is_reg(word addr)
{
	if (0x2000 <= addr && addr < 0x4020)
		return TRUE;
	else
		return FALSE;
}

void
ppu_init()
{
	;
}

