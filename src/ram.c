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

	if (addr == 0x2002)
		return 0xff;

	if (addr < 0x8000)
		ret = ram[addr];
	else if (addr < 0xC000)
		ret = prg_rom.bank[prg_rom.low][addr % 0x4000];
	else
		ret = prg_rom.bank[prg_rom.up][addr % 0x4000];

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
	if (addr < 0x8000)
		ram[addr] = b;
}

void
ram_pushb(byte b)
{
	ram_setb(0x100 + reg.SP, b);
	reg.SP--;
}

void
ram_pushw(word w)
{
	ram_pushb(w >> 8);
	ram_pushb(w % 0x100);
}

byte
ram_popb()
{
	reg.SP++;

	return ram_getb(0x100 + reg.SP);
}

word
ram_popw()
{
	word ret;

	ret = ram_popb();
	ret |= ram_popb() << 8;

	return ret;
}

void
ram_init()
{
	prg_rom.low = 0;
	prg_rom.up = prg_rom.n - 1;
}

