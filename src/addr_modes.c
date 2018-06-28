#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "cpu.h"
#include "ppu.h"
#include "ram.h"


byte cpu_arg;
word cpu_addr;


void
addr_mode_imm()
{
	cpu_arg = ram_getb(reg.PC);
	reg.PC++;
}

void
addr_mode_ind()
{
	cpu_addr = ram_getw(ram_getw(reg.PC));

	reg.PC += 2;

	if (!(ppu_is_reg(cpu_addr) ||
	    ppu_is_reg_r(cpu_addr) ))
		cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_zp()
{
	cpu_addr = ram_getb(reg.PC);
	reg.PC++;
	if (!(ppu_is_reg(cpu_addr) ||
	    ppu_is_reg_r(cpu_addr) ))
		cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_zpx()
{
	cpu_addr = (byte)(ram_getb(reg.PC) + reg.X);
	reg.PC++;
	if (!(ppu_is_reg(cpu_addr) ||
	    ppu_is_reg_r(cpu_addr) ))
		cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_zpy()
{
	cpu_addr = (byte)(ram_getb(reg.PC) + reg.Y);
	reg.PC++;
	if (!(ppu_is_reg(cpu_addr) ||
	    ppu_is_reg_r(cpu_addr) ))
		cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_izx()
{
	cpu_addr = ram_getw((byte)(ram_getb(reg.PC) + reg.X));
	reg.PC++;
	if (!(ppu_is_reg(cpu_addr) ||
	    ppu_is_reg_r(cpu_addr) ))
		cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_izy()
{
	cpu_addr = ram_getw(ram_getb(reg.PC)) + reg.Y;
	reg.PC++;
	cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_abs()
{
	cpu_addr = ram_getw(reg.PC);
	reg.PC += 2;
	if (!(ppu_is_reg(cpu_addr) ||
	    ppu_is_reg_r(cpu_addr) ))
		cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_abx()
{
	cpu_addr = ram_getw(reg.PC) + reg.X;
	reg.PC += 2;
	cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_aby()
{
	cpu_addr = ram_getw(reg.PC) + reg.Y;
	reg.PC += 2;
	cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_rel()
{
	cpu_addr = reg.PC + 1 + (sbyte)ram_getb(reg.PC);
	reg.PC++;
	cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_a()
{
	cpu_arg = reg.A;
}

