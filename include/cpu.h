#ifndef _CPU_H_
#define _CPU_H_

#include "common.h"


struct ines_header {
	byte magick[4];

	/* In 16 Kb */
	byte prg_rom_num;
	/* In 8 Kb */
	byte chr_rom_num;

	byte flag6;
	byte flag7;

	/* In 8 Kb */
	byte prg_ram_num;

	byte flag9;

	byte res[6];
};

struct cpu {
	word PC;
	byte SP;
	sbyte A;
	byte X;
	byte Y;

	union {
		struct {
			byte C	: 1;
			byte Z	: 1;
			byte I	: 1;
			/* Flag D is ignored */
			byte D	: 1;
			byte B	: 1;
			byte RES: 1;
			byte V	: 1;
			byte N	: 1;
		} __attribute__ ((packed)) fl;
		byte n;
	} P;
};

extern struct cpu reg;
extern struct ines_header hdr;

extern byte mapper;

extern int cpu_cycles;

/* For opcodes */
extern word cpu_addr;
extern byte cpu_arg;

void cpu_init();
void cpu_run_cycles(int n);

void cpu_irq();
void cpu_nmi();

byte cpu_is_reg(word addr);


#endif /* _CPU_H_ */
