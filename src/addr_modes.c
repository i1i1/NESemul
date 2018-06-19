#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "cpu.h"
#include "ram.h"

byte cpu_arg;
word cpu_addr;


#define todo() do {\
		fprintf(stderr, "\nerror: %s function"	\
			" unfinished at line %d\n\n",	\
			__FUNCTION__, __LINE__);	\
		exit(1);				\
	} while(0)

void
addr_mode_imm()
{
	cpu_arg = ram_getb(reg.PC);
	reg.PC++;
}

void
addr_mode_ind()
{
	todo();
}

void
addr_mode_zp()
{
	cpu_addr = ram_getb(reg.PC);
	reg.PC++;
	cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_zpx()
{
	todo();
}

void
addr_mode_zpy()
{
	todo();
}

void
addr_mode_izx()
{
	todo();
}

void
addr_mode_izy()
{
	todo();
}

void
addr_mode_abs()
{
	cpu_addr = ram_getw(reg.PC);

	reg.PC += 2;

	cpu_arg = ram_getb(cpu_addr);
}

void
addr_mode_abx()
{
	todo();
}

void
addr_mode_aby()
{
	todo();
}

void
addr_mode_rel()
{
	cpu_addr = reg.PC + ram_getb(reg.PC);
	cpu_arg = ram_getb(cpu_addr);
}

