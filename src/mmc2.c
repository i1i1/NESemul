#include <stdio.h>

#include "common.h"
#include "mapper_defs.h"
#include "mmc2.h"
#include "cpu.h"
#include "ram.h"

void
mmc2_init()
{
	prg_rom.low = 1;
	prg_rom.up = prg_rom.n - 1;
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
	else if (ram_getb(addr) == b) {
		printf("Changing lower bank to %d!\n", b);
		prg_rom.low = b;
	}
}

