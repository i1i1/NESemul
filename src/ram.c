#include "common.h"
#include "cpu.h"
#include "mapper_defs.h"
#include "ram.h"

struct prg_rom prg_rom = { NULL, 0, 0, 0 };
byte ram[0x10000] = { 0 };

byte
ram_getb(word addr)
{
	return map[mapper].getb(addr);
}

word
ram_getw(word addr)
{
	return (ram_getb(addr + 1) << 8) | ram_getb(addr);
}

byte
ram_general_getb(word addr)
{
	byte ret;

	switch (addr / 0x4000) {
	case 0:
	case 1:
		ret = ram[addr];
	case 2:
		ret = prg_rom.bank[prg_rom.low][addr % 0x4000];
	case 3:
		ret = prg_rom.bank[prg_rom.up][addr % 0x4000];
	}

	return ret;
}

void
ram_setb(word addr, byte b)
{
	map[mapper].setb(addr, b);
}

void
ram_general_setb(word addr, byte b)
{
	switch (addr / 0x4000) {
	case 0:
	case 1:
		ram[addr] = b;
		/* PRG ROM Read Only! */
	}
}

void
ram_init()
{
	prg_rom.low = 0;
	prg_rom.up = prg_rom.n - 1;
}

