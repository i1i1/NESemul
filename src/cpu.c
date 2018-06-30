#include "cpu.h"
#include "ram.h"
#include "mapper_defs.h"
#include "mmc2.h"

#include "opcodes.h"

struct cpu reg;

int cpu_cycles;

#define MAP(hex, init_, getb_, setb_)		\
	[(hex)] = {				\
		.init = (init_),		\
		.getb = (getb_),		\
		.setb = (setb_)			\
	},

struct mapper map[256] = {
	MAP(2, mmc2_init, mmc2_getb, mmc2_setb)
};

#undef MAP

/* Some ugly prints, what produce nice output */
void
printinfo()
{
	byte op, i;

	printf("%04x:\t", reg.PC);

	op = ram_getb(reg.PC);

	for (i = 0; i < ops[op].len; i++)
		printf(" %02x", ram_getb(reg.PC + i));

	if (i < 3)
		printf("\t");

	printf("\t");

	printf("%s", ops[op].cname);
	if (ops[op].mode)
		printf(" %s", ops[op].mname);

	printf("\t\t(A=%02x;\tX=%02x;\tY=%02x;\tSP=%02x) "
	       "(P=%02x; .C=%d .Z=%d .O=%d .N=%d) (cycle %d)\n",
	       (byte)reg.A, reg.X, reg.Y, reg.SP,
	       reg.P.n, reg.P.fl.C, reg.P.fl.Z,
	       reg.P.fl.V, reg.P.fl.N, cpu_cycles);
}

void
cpu_run_cycles(int n)
{
	byte op;

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
	printf("\nIRQ Occured!\n\n");

	ram_pushw(reg.PC);
	ram_pushb(reg.P.n);

	reg.P.fl.I = 1;

	reg.PC = ram_getw(0xFFFE);
}

void
cpu_nmi()
{
	printf("\nNMI Occured!\n\n");

	ram_pushw(reg.PC);
	ram_pushb(reg.P.n);

	reg.P.fl.I = 1;

	reg.PC = ram_getw(0xFFFA);
}

