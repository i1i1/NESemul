#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "ram.h"

#include "common.h"
#include "addr_modes.h"

#include "opcodes.h"


/* Some helpfull rutines for setting reg.P flags */
static void
flag_neg(sbyte n)
{
	if (n < 0)
		reg.P.fl.N = 1;
	else
		reg.P.fl.N = 0;
}

static void
flag_zero(sbyte n)
{
	if (n == 0)
		reg.P.fl.Z = 1;
	else
		reg.P.fl.Z = 0;
}

static void
flag_carry(sbyte n)
{
	if (n != 0)
		reg.P.fl.C = 1;
	else
		reg.P.fl.C = 0;
}

static void
flag_over(int n)
{
	if (-127 < n || n > 128)
		reg.P.fl.V = 1;
	else
		reg.P.fl.V = 0;
}


static void
op_nop()
{
}

static void
op_jmp()
{
	printf("\n");
	reg.PC = cpu_addr;
}

static void
op_brk()
{
	reg.PC++;
	reg.P.fl.B = 1;

	cpu_irq();
}

static void
op_ora()
{
	reg.A |= ram_getb(cpu_addr);

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_asl()
{
	byte b;

	b = ram_getb(cpu_addr);

	flag_carry(b >> 7);
	flag_zero(b << 1);
	flag_neg(b << 1);

	ram_setb(cpu_addr, b << 1);
}

static void
op_asl_a()
{
	flag_carry(reg.A >> 7);
	flag_zero(reg.A << 1);
	flag_neg(reg.A << 1);

	reg.A <<= 1;
}

static void
op_php()
{
	ram_pushb(reg.P.n);
}

static void
op_bpl()
{
	if (reg.P.fl.N == 0)
		op_jmp();
}

static void
op_clc()
{
	reg.P.fl.C = 0;
}

static void
op_jsr()
{
	ram_pushw(reg.PC - 1);
	op_jmp();
}

static void
op_and()
{
	reg.A &= ram_getb(cpu_addr);

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_bit()
{
	byte b;

	b = ram_getb(cpu_addr);

	reg.P.fl.N = b >> 7;
	reg.P.fl.V = b >> 6;

	flag_zero(reg.A & b);
}

static void
op_rol()
{
	int c;
	byte b;

	b = ram_getb(cpu_addr);
	c = b >> 7;
	b <<= 1;
	b |= reg.P.fl.C;
	reg.P.fl.C = c;

	flag_neg(b);
	flag_zero(b);

	ram_setb(cpu_addr, b);
}

static void
op_rol_a()
{
	int c;

	c = reg.A >> 7;
	reg.A <<= 1;
	reg.A |= reg.P.fl.C;
	reg.P.fl.C = c;

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_plp()
{
	reg.P.n = ram_popb();
	reg.P.fl.B = 0;
}

static void
op_bmi()
{
	if (reg.P.fl.N)
		op_jmp();
}

static void
op_sec()
{
	reg.P.fl.C = 1;
}

static void
op_rti()
{
	printf("\n");
	reg.P.n = ram_popb();
	reg.PC = ram_popw();
}

static void
op_eor()
{
	reg.A ^= ram_getb(cpu_addr);

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_lsr()
{
	byte b;

	b = ram_getb(cpu_addr);

	flag_carry(b % 2);
	flag_zero(b >> 1);
	flag_neg(b >> 1);

	ram_setb(cpu_addr, b >> 1);
}

static void
op_lsr_a()
{
	reg.P.fl.C = reg.A % 2;
	reg.A >>= 1;

	flag_zero(reg.A);
	flag_neg(reg.A);
}

static void
op_pha()
{
	ram_pushb(reg.A);
}

static void
op_bvc()
{
	if (reg.P.fl.V == 0)
		op_jmp();
}

static void
op_cli()
{
	reg.P.fl.I = 0;
}

static void
op_rts()
{
	reg.PC = ram_popw() + 1;
	printf("\n");
}

static void
op_adc()
{
	int res;

	res = ram_getb(cpu_addr) + (byte)reg.A + reg.P.fl.C;

	flag_zero(res);
	if (res > 256)
		reg.P.fl.C = 1;
	else
		reg.P.fl.C = 0;
	flag_over(res);
	flag_neg(res);

	reg.A = res;
}

static void
op_ror()
{
	int c;
	byte b;

	b = ram_getb(cpu_addr);
	c = b % 2;
	b >>= 1;
	b |= reg.P.fl.C << 7;
	reg.P.fl.C = c;

	flag_neg(b);
	flag_zero(b);

	ram_setb(cpu_addr, b);
}

static void
op_ror_a()
{
	int c;

	c = reg.A % 2;
	reg.A >>= 1;
	reg.A |= reg.P.fl.C << 7;
	reg.P.fl.C = c;

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_pla()
{
	reg.A = ram_popb();
}

static void
op_bvs()
{
	if (reg.P.fl.V)
		op_jmp();
}

static void
op_sei()
{
	reg.P.fl.I = 1;
}

static void
op_sta()
{
	ram_setb(cpu_addr, reg.A);
}

static void
op_stx()
{
	ram_setb(cpu_addr, reg.X);
}

static void
op_sty()
{
	ram_setb(cpu_addr, reg.Y);
}

static void
op_dey()
{
	reg.Y--;

	flag_neg(reg.Y);
	flag_zero(reg.Y);
}

static void
op_txa()
{
	reg.A = reg.X;

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_bcc()
{
	if (reg.P.fl.C == 0)
		op_jmp();
}

static void
op_tya()
{
	reg.A = reg.Y;

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_txs()
{
	reg.SP = reg.X;
}

static void
op_ldy()
{
	reg.Y = ram_getb(cpu_addr);

	flag_neg(reg.Y);
	flag_zero(reg.Y);
}

static void
op_lda()
{
	reg.A = ram_getb(cpu_addr);

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_ldx()
{
	reg.X = ram_getb(cpu_addr);

	flag_neg(reg.X);
	flag_zero(reg.X);
}

static void
op_tay()
{
	reg.Y = reg.A;

	flag_neg(reg.Y);
	flag_zero(reg.Y);
}

static void
op_tax()
{
	reg.X = reg.A;

	flag_neg(reg.X);
	flag_zero(reg.X);
}

static void
op_bcs()
{
	if (reg.P.fl.C)
		op_jmp();
}

static void
op_clv()
{
	reg.P.fl.V = 0;
}

static void
op_tsx()
{
	reg.X = reg.SP;
}

static void
op_cpy()
{
	byte b;

	b = ram_getb(cpu_addr);

	if (reg.Y == b) {
		reg.P.fl.N = 0;
		reg.P.fl.Z = 1;
		reg.P.fl.C = 1;
	} else if (reg.Y < b) {
		reg.P.fl.N = 1;
		reg.P.fl.Z = 0;
		reg.P.fl.C = 0;
	} else {
		reg.P.fl.N = 0;
		reg.P.fl.Z = 0;
		reg.P.fl.C = 1;
	}
}

static void
op_cmp()
{
	sbyte sb;

	sb = (sbyte)ram_getb(cpu_addr);

	if (reg.A == sb) {
		reg.P.fl.N = 0;
		reg.P.fl.Z = 1;
		reg.P.fl.C = 1;
	} else if (reg.A < sb) {
		reg.P.fl.N = 1;
		reg.P.fl.Z = 0;
		reg.P.fl.C = 0;
	} else {
		reg.P.fl.N = 0;
		reg.P.fl.Z = 0;
		reg.P.fl.C = 1;
	}
}

static void
op_dec()
{
	sbyte n;

	n = ram_getb(cpu_addr) - 1;

	ram_setb(cpu_addr, n);

	flag_neg(n);
	flag_zero(n);
}

static void
op_iny()
{
	reg.Y++;

	flag_neg(reg.Y);
	flag_zero(reg.Y);
}

static void
op_dex()
{
	reg.X--;

	flag_neg(reg.X);
	flag_zero(reg.X);
}

static void
op_bne()
{
	if (reg.P.fl.Z == 0)
		op_jmp();
}

static void
op_cld()
{
	reg.P.fl.D = 0;
}

static void
op_cpx()
{
	byte b;

	b = ram_getb(cpu_addr);

	if (reg.X < b) {
		reg.P.fl.N = 1;
		reg.P.fl.Z = 0;
		reg.P.fl.C = 0;
	} else if (reg.X == b) {
		reg.P.fl.N = 0;
		reg.P.fl.Z = 1;
		reg.P.fl.C = 1;
	} else {
		reg.P.fl.N = 0;
		reg.P.fl.Z = 0;
		reg.P.fl.C = 1;
	}
}

static void
op_sbc()
{
	int res;

	res = reg.A - ram_getb(cpu_addr) - (1 - reg.P.fl.C);

	flag_zero(res);
	flag_carry(res);
	flag_over(res);
	flag_neg(res);

	reg.A = res;
}

static void
op_inc()
{
	sbyte n;

	n = ram_getb(cpu_addr) + 1;

	flag_neg(n);
	flag_zero(n);

	ram_setb(cpu_addr, n);
}

static void
op_inx()
{
	reg.X++;

	flag_neg(reg.X);
	flag_zero(reg.X);
}

static void
op_beq()
{
	if (reg.P.fl.Z)
		op_jmp();
}

static void
op_sed()
{
	reg.P.fl.D = 1;
}

/* Unofficial opcodes go from here */

static void
op_slo()
{
	todo();
}

static void
op_anc()
{
	todo();
}

static void
op_rla()
{
	todo();
}

static void
op_sre()
{
	todo();
}

static void
op_alr()
{
	todo();
}

static void
op_rra()
{
	todo();
}

static void
op_arr()
{
	todo();
}

static void
op_sax()
{
	todo();
}

static void
op_xaa()
{
	todo();
}

static void
op_ahx()
{
	todo();
}

static void
op_tas()
{
	todo();
}

static void
op_shy()
{
	todo();
}

static void
op_shx()
{
	todo();
}

static void
op_lax()
{
	todo();
}

static void
op_las()
{
	todo();
}

static void
op_dcp()
{
	todo();
}

static void
op_axs()
{
	todo();
}

static void
op_isc()
{
	todo();
}

#undef todo

#define OP(hex, comm, md, cycl, ln) [(hex)] = {		\
		.cmd	= (comm),			\
		.cname	= 3 + #comm,			\
		.mode	= (md),				\
		.mname	= 10 + #md,			\
		.cycles	= (cycl),			\
		.len	= ln,				\
	},

struct opcode ops[256] =
{
	OP(0x00,	op_brk,		addr_mode_imp,	7,	1)
	OP(0x01,	op_ora,		addr_mode_izx,	6,	2)
	OP(0x03,	op_slo,		addr_mode_izx,	8,	2)
	OP(0x04,	op_nop,		addr_mode_imp,	3,	1)
	OP(0x05,	op_ora,		addr_mode_zp,	3,	2)
	OP(0x06,	op_asl,		addr_mode_zp,	5,	2)
	OP(0x07,	op_slo,		addr_mode_zp,	5,	2)
	OP(0x08,	op_php,		addr_mode_imp,	3,	1)
	OP(0x09,	op_ora,		addr_mode_imm,	2,	2)
	OP(0x0A,	op_asl_a,	addr_mode_imp,	2,	1)
	OP(0x0B,	op_anc,		addr_mode_imm,	2,	2)
	OP(0x0C,	op_nop,		addr_mode_imp,	4,	1)
	OP(0x0D,	op_ora,		addr_mode_abs,	4,	3)
	OP(0x0E,	op_asl,		addr_mode_abs,	6,	3)
	OP(0x0F,	op_slo,		addr_mode_abs,	6,	3)
	OP(0x10,	op_bpl,		addr_mode_rel,	3,	2)
	OP(0x11,	op_ora,		addr_mode_izy,	5,	2)
	OP(0x13,	op_slo,		addr_mode_izy,	8,	2)
	OP(0x14,	op_nop,		addr_mode_imp,	4,	1)
	OP(0x15,	op_ora,		addr_mode_zpx,	4,	2)
	OP(0x16,	op_asl,		addr_mode_zpx,	6,	2)
	OP(0x17,	op_slo,		addr_mode_zpx,	6,	2)
	OP(0x18,	op_clc,		addr_mode_imp,	2,	1)
	OP(0x19,	op_ora,		addr_mode_aby,	4,	3)
	OP(0x1A,	op_nop,		addr_mode_imp,	2,	1)
	OP(0x1B,	op_slo,		addr_mode_aby,	7,	3)
	OP(0x1C,	op_nop,		addr_mode_imp,	4,	1)
	OP(0x1D,	op_ora,		addr_mode_abx,	4,	3)
	OP(0x1E,	op_asl,		addr_mode_abx,	7,	3)
	OP(0x1F,	op_slo,		addr_mode_abx,	7,	3)
	OP(0x20,	op_jsr,		addr_mode_abs,	6,	3)
	OP(0x21,	op_and,		addr_mode_izx,	6,	2)
	OP(0x23,	op_rla,		addr_mode_izx,	8,	2)
	OP(0x24,	op_bit,		addr_mode_zp,	3,	2)
	OP(0x25,	op_and,		addr_mode_zp,	3,	2)
	OP(0x26,	op_rol,		addr_mode_zp,	5,	2)
	OP(0x27,	op_rla,		addr_mode_zp,	5,	2)
	OP(0x28,	op_plp,		addr_mode_imp,	4,	1)
	OP(0x29,	op_and,		addr_mode_imm,	2,	2)
	OP(0x2A,	op_rol_a,	addr_mode_imp,	2,	1)
	OP(0x2C,	op_bit,		addr_mode_abs,	4,	3)
	OP(0x2D,	op_and,		addr_mode_abs,	4,	3)
	OP(0x2E,	op_rol,		addr_mode_abs,	6,	3)
	OP(0x2F,	op_rla,		addr_mode_abs,	6,	3)
	OP(0x30,	op_bmi,		addr_mode_rel,	2,	2)
	OP(0x31,	op_and,		addr_mode_izy,	5,	2)
	OP(0x33,	op_rla,		addr_mode_izy,	8,	2)
	OP(0x35,	op_and,		addr_mode_zpx,	4,	2)
	OP(0x36,	op_rol,		addr_mode_zpx,	6,	2)
	OP(0x37,	op_rla,		addr_mode_zpx,	6,	2)
	OP(0x38,	op_sec,		addr_mode_imp,	2,	1)
	OP(0x39,	op_and,		addr_mode_aby,	4,	3)
	OP(0x3B,	op_rla,		addr_mode_aby,	7,	3)
	OP(0x3D,	op_and,		addr_mode_abx,	4,	3)
	OP(0x3E,	op_rol,		addr_mode_abx,	7,	3)
	OP(0x3F,	op_rla,		addr_mode_abx,	7,	3)
	OP(0x40,	op_rti,		addr_mode_imp,	6,	1)
	OP(0x41,	op_eor,		addr_mode_izx,	6,	2)
	OP(0x43,	op_sre,		addr_mode_izx,	8,	2)
	OP(0x45,	op_eor,		addr_mode_zp,	3,	2)
	OP(0x46,	op_lsr,		addr_mode_zp,	5,	2)
	OP(0x47,	op_sre,		addr_mode_zp,	5,	2)
	OP(0x48,	op_pha,		addr_mode_imp,	3,	1)
	OP(0x49,	op_eor,		addr_mode_imm,	2,	2)
	OP(0x4A,	op_lsr_a,	addr_mode_imp,	2,	1)
	OP(0x4B,	op_alr,		addr_mode_imm,	2,	2)
	OP(0x4C,	op_jmp,		addr_mode_abs,	3,	3)
	OP(0x4D,	op_eor,		addr_mode_abs,	4,	3)
	OP(0x4E,	op_lsr,		addr_mode_abs,	6,	3)
	OP(0x4F,	op_sre,		addr_mode_abs,	6,	3)
	OP(0x50,	op_bvc,		addr_mode_rel,	3,	2)
	OP(0x51,	op_eor,		addr_mode_izy,	5,	2)
	OP(0x53,	op_sre,		addr_mode_izy,	8,	2)
	OP(0x55,	op_eor,		addr_mode_zpx,	4,	2)
	OP(0x56,	op_lsr,		addr_mode_zpx,	6,	2)
	OP(0x57,	op_sre,		addr_mode_zpx,	6,	2)
	OP(0x58,	op_cli,		addr_mode_imp,	2,	1)
	OP(0x59,	op_eor,		addr_mode_aby,	4,	3)
	OP(0x5B,	op_sre,		addr_mode_aby,	7,	3)
	OP(0x5D,	op_eor,		addr_mode_abx,	4,	3)
	OP(0x5E,	op_lsr,		addr_mode_abx,	7,	3)
	OP(0x5F,	op_sre,		addr_mode_abx,	7,	3)
	OP(0x60,	op_rts,		addr_mode_imp,	6,	1)
	OP(0x61,	op_adc,		addr_mode_izx,	6,	2)
	OP(0x63,	op_rra,		addr_mode_izx,	8,	2)
	OP(0x65,	op_adc,		addr_mode_zp,	3,	2)
	OP(0x66,	op_ror,		addr_mode_zp,	5,	2)
	OP(0x67,	op_rra,		addr_mode_zp,	5,	2)
	OP(0x68,	op_pla,		addr_mode_imp,	4,	1)
	OP(0x69,	op_adc,		addr_mode_imm,	2,	2)
	OP(0x6A,	op_ror_a,	addr_mode_imp,	2,	1)
	OP(0x6B,	op_arr,		addr_mode_imm,	2,	2)
	OP(0x6C,	op_jmp,		addr_mode_ind,	5,	3)
	OP(0x6D,	op_adc,		addr_mode_abs,	4,	2)
	OP(0x6E,	op_ror,		addr_mode_abs,	6,	2)
	OP(0x6F,	op_rra,		addr_mode_abs,	6,	2)
	OP(0x70,	op_bvs,		addr_mode_rel,	2,	2)
	OP(0x71,	op_adc,		addr_mode_izy,	5,	2)
	OP(0x73,	op_rra,		addr_mode_izy,	8,	2)
	OP(0x75,	op_adc,		addr_mode_zpx,	4,	2)
	OP(0x76,	op_ror,		addr_mode_zpx,	6,	2)
	OP(0x77,	op_rra,		addr_mode_zpx,	6,	2)
	OP(0x78,	op_sei,		addr_mode_imp,	2,	1)
	OP(0x79,	op_adc,		addr_mode_aby,	4,	3)
	OP(0x7B,	op_rra,		addr_mode_aby,	7,	3)
	OP(0x7D,	op_adc,		addr_mode_abx,	4,	3)
	OP(0x7E,	op_ror,		addr_mode_abx,	7,	3)
	OP(0x7F,	op_rra,		addr_mode_abx,	7,	3)
	OP(0x80,	op_nop,		addr_mode_imp,	2,	1)
	OP(0x81,	op_sta,		addr_mode_izx,	6,	2)
	OP(0x83,	op_sax,		addr_mode_izx,	6,	2)
	OP(0x84,	op_sty,		addr_mode_zp,	3,	2)
	OP(0x85,	op_sta,		addr_mode_zp,	3,	2)
	OP(0x86,	op_stx,		addr_mode_zp,	3,	2)
	OP(0x87,	op_sax,		addr_mode_zp,	3,	2)
	OP(0x88,	op_dey,		addr_mode_imp,	2,	1)
	OP(0x8A,	op_txa,		addr_mode_imp,	2,	1)
	OP(0x8B,	op_xaa,		addr_mode_imm,	2,	2)
	OP(0x8C,	op_sty,		addr_mode_abs,	4,	3)
	OP(0x8D,	op_sta,		addr_mode_abs,	4,	3)
	OP(0x8E,	op_stx,		addr_mode_abs,	4,	3)
	OP(0x8F,	op_sax,		addr_mode_abs,	4,	3)
	OP(0x90,	op_bcc,		addr_mode_rel,	3,	2)
	OP(0x91,	op_sta,		addr_mode_izy,	6,	2)
	OP(0x93,	op_ahx,		addr_mode_izy,	6,	2)
	OP(0x94,	op_sty,		addr_mode_zpx,	4,	2)
	OP(0x95,	op_sta,		addr_mode_zpx,	4,	2)
	OP(0x96,	op_stx,		addr_mode_zpy,	4,	2)
	OP(0x97,	op_sax,		addr_mode_zpy,	4,	2)
	OP(0x98,	op_tya,		addr_mode_imp,	2,	1)
	OP(0x99,	op_sta,		addr_mode_aby,	5,	3)
	OP(0x9A,	op_txs,		addr_mode_imp,	2,	1)
	OP(0x9B,	op_tas,		addr_mode_aby,	5,	3)
	OP(0x9C,	op_shy,		addr_mode_abx,	5,	3)
	OP(0x9D,	op_sta,		addr_mode_abx,	5,	3)
	OP(0x9E,	op_shx,		addr_mode_aby,	5,	3)
	OP(0x9F,	op_ahx,		addr_mode_aby,	5,	3)
	OP(0xA0,	op_ldy,		addr_mode_imm,	2,	2)
	OP(0xA1,	op_lda,		addr_mode_izx,	6,	2)
	OP(0xA2,	op_ldx,		addr_mode_imm,	2,	2)
	OP(0xA3,	op_lax,		addr_mode_izx,	6,	2)
	OP(0xA4,	op_ldy,		addr_mode_zp,	3,	2)
	OP(0xA5,	op_lda,		addr_mode_zp,	3,	2)
	OP(0xA6,	op_ldx,		addr_mode_zp,	3,	2)
	OP(0xA7,	op_lax,		addr_mode_zp,	3,	2)
	OP(0xA8,	op_tay,		addr_mode_imp,	2,	1)
	OP(0xA9,	op_lda,		addr_mode_imm,	2,	2)
	OP(0xAA,	op_tax,		addr_mode_imp,	2,	1)
	OP(0xAB,	op_lax,		addr_mode_imm,	2,	2)
	OP(0xAC,	op_ldy,		addr_mode_abs,	4,	3)
	OP(0xAD,	op_lda,		addr_mode_abs,	4,	3)
	OP(0xAE,	op_ldx,		addr_mode_abs,	4,	3)
	OP(0xAF,	op_lax,		addr_mode_abs,	4,	3)
	OP(0xB0,	op_bcs,		addr_mode_rel,	2,	2)
	OP(0xB1,	op_lda,		addr_mode_izy,	5,	2)
	OP(0xB3,	op_lax,		addr_mode_izy,	5,	2)
	OP(0xB4,	op_ldy,		addr_mode_zpx,	4,	2)
	OP(0xB5,	op_lda,		addr_mode_zpx,	4,	2)
	OP(0xB6,	op_ldx,		addr_mode_zpy,	4,	2)
	OP(0xB7,	op_lax,		addr_mode_zpy,	4,	2)
	OP(0xB8,	op_clv,		addr_mode_imp,	2,	1)
	OP(0xB9,	op_lda,		addr_mode_aby,	4,	3)
	OP(0xBA,	op_tsx,		addr_mode_imp,	2,	1)
	OP(0xBB,	op_las,		addr_mode_aby,	4,	3)
	OP(0xBC,	op_ldy,		addr_mode_abx,	4,	3)
	OP(0xBD,	op_lda,		addr_mode_abx,	4,	3)
	OP(0xBE,	op_ldx,		addr_mode_aby,	4,	3)
	OP(0xBF,	op_lax,		addr_mode_aby,	4,	3)
	OP(0xC0,	op_cpy,		addr_mode_imm,	2,	2)
	OP(0xC1,	op_cmp,		addr_mode_izx,	6,	2)
	OP(0xC3,	op_dcp,		addr_mode_izx,	8,	2)
	OP(0xC4,	op_cpy,		addr_mode_zp,	3,	2)
	OP(0xC5,	op_cmp,		addr_mode_zp,	3,	2)
	OP(0xC6,	op_dec,		addr_mode_zp,	5,	2)
	OP(0xC7,	op_dcp,		addr_mode_zp,	5,	2)
	OP(0xC8,	op_iny,		addr_mode_imp,	2,	1)
	OP(0xC9,	op_cmp,		addr_mode_imm,	2,	2)
	OP(0xCA,	op_dex,		addr_mode_imp,	2,	1)
	OP(0xCB,	op_axs,		addr_mode_imm,	2,	2)
	OP(0xCC,	op_cpy,		addr_mode_abs,	4,	3)
	OP(0xCD,	op_cmp,		addr_mode_abs,	4,	3)
	OP(0xCE,	op_dec,		addr_mode_abs,	6,	3)
	OP(0xCF,	op_dcp,		addr_mode_abs,	6,	3)
	OP(0xD0,	op_bne,		addr_mode_rel,	3,	2)
	OP(0xD1,	op_cmp,		addr_mode_izy,	5,	2)
	OP(0xD3,	op_dcp,		addr_mode_izy,	8,	2)
	OP(0xD5,	op_cmp,		addr_mode_zpx,	4,	2)
	OP(0xD6,	op_dec,		addr_mode_zpx,	6,	2)
	OP(0xD7,	op_dcp,		addr_mode_zpx,	6,	2)
	OP(0xD8,	op_cld,		addr_mode_imp,	2,	1)
	OP(0xD9,	op_cmp,		addr_mode_aby,	4,	3)
	OP(0xDB,	op_dcp,		addr_mode_aby,	7,	3)
	OP(0xDD,	op_cmp,		addr_mode_abx,	4,	3)
	OP(0xDE,	op_dec,		addr_mode_abx,	7,	3)
	OP(0xDF,	op_dcp,		addr_mode_abx,	7,	3)
	OP(0xE0,	op_cpx,		addr_mode_imm,	2,	2)
	OP(0xE1,	op_sbc,		addr_mode_izx,	6,	2)
	OP(0xE3,	op_isc,		addr_mode_izx,	8,	2)
	OP(0xE4,	op_cpx,		addr_mode_zp,	3,	2)
	OP(0xE5,	op_sbc,		addr_mode_zp,	3,	2)
	OP(0xE6,	op_inc,		addr_mode_zp,	5,	2)
	OP(0xE7,	op_isc,		addr_mode_zp,	5,	2)
	OP(0xE8,	op_inx,		addr_mode_imp,	2,	1)
	OP(0xE9,	op_sbc,		addr_mode_imm,	2,	2)
	OP(0xEA,	op_nop,		addr_mode_imp,	2,	1)
	OP(0xEC,	op_cpx,		addr_mode_abs,	4,	3)
	OP(0xED,	op_sbc,		addr_mode_abs,	4,	3)
	OP(0xEE,	op_inc,		addr_mode_abs,	6,	3)
	OP(0xEF,	op_isc,		addr_mode_abs,	6,	3)
	OP(0xF0,	op_beq,		addr_mode_rel,	2,	2)
	OP(0xF1,	op_sbc,		addr_mode_izy,	5,	2)
	OP(0xF3,	op_isc,		addr_mode_izy,	8,	2)
	OP(0xF5,	op_sbc,		addr_mode_zpx,	4,	2)
	OP(0xF6,	op_inc,		addr_mode_zpx,	6,	2)
	OP(0xF7,	op_isc,		addr_mode_zpx,	6,	2)
	OP(0xF8,	op_sed,		addr_mode_imp,	2,	1)
	OP(0xF9,	op_sbc,		addr_mode_aby,	4,	3)
	OP(0xFB,	op_isc,		addr_mode_aby,	7,	3)
	OP(0xFD,	op_sbc,		addr_mode_abx,	4,	3)
	OP(0xFE,	op_inc,		addr_mode_abx,	7,	3)
	OP(0xFF,	op_isc,		addr_mode_abx,	7,	3)
};

#undef OP

