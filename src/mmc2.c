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
	else if (ram_getb(addr) == b) {
		printf("\nChanging upper bank to %d!\n", b % 16 - 1);
		prg_rom.up = b % 8 - 1;
	}
}

