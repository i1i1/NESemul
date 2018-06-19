#ifndef _CPU_H_
#define _CPU_H_

#include "common.h"


struct cpu {
	word PC;
	byte SP;
	sbyte A;
	byte X;
	byte Y;

	struct {
		byte C	: 1;
		byte Z	: 1;
		byte I	: 1;
		byte D	: 1;
		byte B	: 1;
		byte RES: 1;
		byte V	: 1;
		byte N	: 1;
	} P;
};

extern struct cpu reg;
/* For opcodes */
extern word cpu_addr;
extern byte cpu_arg;

void cpu_init();


#endif /* _CPU_H_ */
