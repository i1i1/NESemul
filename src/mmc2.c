#include <stdio.h>

#include "common.h"
#include "mapper_defs.h"
#include "mmc2.h"
#include "cpu.h"
#include "ram.h"

void
mmc2_init()
{
	printf("\tMapper 2!\n\n");
}

byte
mmc2_getb(word addr)
{
	return ram_general_getb(addr);
}

void
mmc2_setb(word addr, byte b)
{
	if (addr < 0x8000)
		ram_general_setb(addr, b);
	else
		prg_rom.low = b % prg_rom.n;
}

