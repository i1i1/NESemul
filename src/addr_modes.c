#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "cpu.h"
#include "ppu.h"
#include "ram.h"


word cpu_addr;


void
addr_mode_ind()
{
	word a;

	a = ram_getw(reg.PC);

	/* Bug implementation! */
	if (a % 0x100 == 0xFF)
		cpu_addr = (ram_getb(a & 0xFF00) << 8) | ram_getb(a);
	else
		cpu_addr = ram_getw(a);

	reg.PC += 2;
}

void
addr_mode_zp()
{
	cpu_addr = ram_getb(reg.PC);
	reg.PC++;
}

void
addr_mode_zpx()
{
	cpu_addr = (byte)(ram_getb(reg.PC) + reg.X);
	reg.PC++;
}

void
addr_mode_zpy()
{
	cpu_addr = (byte)(ram_getb(reg.PC) + reg.Y);
	reg.PC++;
}

void
addr_mode_izx()
{
	byte a;
	byte low, high;

	a = ram_getb(reg.PC);
	high = ram_getb((byte)(a + reg.X + 1));
	low = ram_getb((byte)(a + reg.X));
	cpu_addr = (high << 8) | low;
	printf("izx addr %04x\n", cpu_addr);

	reg.PC++;
}

void
addr_mode_izy()
{
	byte a;
	byte low, high;

	a = ram_getb(reg.PC);
	high = ram_getb((byte)(a + 1));
	low = ram_getb(a);
	cpu_addr = ((high << 8) | low) + reg.Y;

	printf("izy addr %04x\n", cpu_addr);

	reg.PC++;
}

void
addr_mode_abs()
{
	cpu_addr = ram_getw(reg.PC);
	reg.PC += 2;
}

void
addr_mode_abx()
{
	cpu_addr = ram_getw(reg.PC) + reg.X;
	reg.PC += 2;
}

void
addr_mode_aby()
{
	cpu_addr = ram_getw(reg.PC) + reg.Y;
	reg.PC += 2;
}

void
addr_mode_rel()
{
	cpu_addr = reg.PC + 1 + (sbyte)ram_getb(reg.PC);
	reg.PC++;
}

void
addr_mode_imm()
{
	cpu_addr = reg.PC++;
}

void
addr_mode_imp()
{
}

