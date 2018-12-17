#include <stdio.h>

#include "common.h"
#include "mapper_defs.h"
#include "uxrom.h"
#include "cpu.h"
#include "ram.h"

void
uxrom_init()
{
	prg_rom.low = 0;
	prg_rom.up = prg_rom.n - 1;
	chr_rom.cur = 0;
	printf("\tMapper 2!\n\n");
}

byte
uxrom_getb(word addr)
{
	return ram_general_getb(addr);
}

void
uxrom_setb(word addr, byte b)
{
	if (addr < 0x8000)
		ram_general_setb(addr, b);
	else if (ram_getb(addr) == b) {
		printf("Changing lower bank to %d!\n", b);
		prg_rom.low = b;
	}
}

