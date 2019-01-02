#include "cpu.h"
#include "ram.h"
#include "mapper_defs.h"
#include "mmc0.h"
#include "uxrom.h"

#include "opcodes.h"

struct cpu reg;

int cpu_cycles;

#define MAP(hex, init_, getb_, setb_)	\
	[(hex)] = {					\
		.init = (init_),		\
		.getb = (getb_),		\
		.setb = (setb_)			\
	},

struct mapper map[256] = {
	MAP(  0,  mmc0_init,  mmc0_getb,  mmc0_setb)
	MAP(  2, uxrom_init, uxrom_getb, uxrom_setb)
};

#undef MAP

/* Some ugly prints, what produce nice output */
void
printinfo()
{
	byte op, i;

	printf("%04X:\t", reg.PC);

	op = ram_getb(reg.PC);

	for (i = 0; i < ops[op].len; i++)
		printf(" %02X", ram_getb(reg.PC + i));

	if (ops[op].len == 1)
		printf("\t\t");

	if (ops[op].len == 2)
		printf("\t");

	printf("\t");

	printf("%s", ops[op].cname);
	if (ops[op].mode)
		printf(" %s", ops[op].mname);

	printf(" ");

	for (i = ops[op].len-1; i > 0; i--)
		printf("%02X", ram_getb(reg.PC + i));

	if (ops[op].len != 3)
		printf("\t");

	printf("\tA:%02X X:%02X Y:%02X P:%02X SP:%02X\n",
	       (byte)reg.A, reg.X, reg.Y, reg.P.n, reg.SP);
}

void
cpu_run_cycles(int n)
{
	byte op;

//	printf("In cpu!\n");
	while (n > 0) {
		printinfo();

		op = ram_getb(reg.PC);
		reg.PC++;

		if (ops[op].cmd == NULL)
			die("Unknown opcode");

		if (ops[op].mode)
			ops[op].mode();

		ops[op].cmd();

		cpu_cycles += ops[op].cycles;
		n -= ops[op].cycles;
	}
}

void
cpu_init()
{
	reg.PC = ram_getw(0xFFFC);

	reg.SP = 0xFD;
	reg.A = reg.X = reg.Y = 0;

	reg.P.n = 0;
	reg.P.fl.I = 1;
	reg.P.fl.RES = 1;

	if (map[mapper].init)
		map[mapper].init();
	else
		die("Unknown mapper");
}

byte
cpu_is_reg(word addr)
{
	if (0x2000 <= addr && addr < 0x4020)
		return TRUE;
	else
		return FALSE;
}

void
cpu_irq()
{
//	printf("\nIRQ Occured!\n\n");

	ram_pushw(reg.PC);
	ram_pushb(reg.P.n);

	reg.P.fl.I = 1;

	reg.PC = ram_getw(0xFFFE);
}

void
cpu_nmi()
{
//	printf("\nNMI Occured!\n\n");

	ram_pushw(reg.PC);
	ram_pushb(reg.P.n);

	reg.P.fl.I = 1;

	reg.PC = ram_getw(0xFFFA);
}

