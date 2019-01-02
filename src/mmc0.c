#include "common.h"
#include "mapper_defs.h"
#include "mmc0.h"
#include "cpu.h"
#include "ram.h"

void
mmc0_init()
{
	prg_rom.low = 0;
	prg_rom.up = prg_rom.n - 1;
	chr_rom.cur = 0;
	dprintf("\tMapper 0!\n\n");
}

byte
mmc0_getb(word addr)
{
	return ram_general_getb(addr);
}

void
mmc0_setb(word addr, byte b)
{
	return ram_general_setb(addr, b);
}
