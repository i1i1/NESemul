#include "cpu.h"
#include "ram.h"
#include "mapper_defs.h"
#include "mmc2.h"

struct cpu reg;

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

extern byte mapper;

void
cpu_init()
{
	reg.PC = ram_getw(0xFFFC);

	reg.SP = 0xFF;
	reg.A = reg.X = reg.Y = 0;
	reg.P.C = reg.P.Z = reg.P.I = reg.P.D
		= reg.P.B = reg.P.V = reg.P.N = 0;

	map[mapper].init();
}

