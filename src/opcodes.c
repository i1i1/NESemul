#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"
#include "ram.h"

#include "common.h"
#include "addr_modes.h"

#include "opcodes.h"


#define todo() do {\
		fprintf(stderr, "\nerror: %s function"	\
			" unfinished at line %d\n\n",	\
			__FUNCTION__, __LINE__);	\
		exit(1);				\
	} while(0)

/* Some helpfull rutines for setting reg.P flags */
static void
flag_neg(sbyte n)
{
	if (n < 0)
		reg.P.N = 1;
}

static void
flag_zero(sbyte n)
{
	if (n == 0)
		reg.P.Z = 1;
}

static void
flag_carry(sbyte n)
{
	if (n)
		reg.P.C = 1;
}


static void
op_nop()
{
}

static void
op_brk()
{
	todo();
}

static void
op_ora()
{
	todo();
}

static void
op_slo()
{
	todo();
}

static void
op_asl()
{
	todo();
}

static void
op_php()
{
	todo();
}

static void
op_anc()
{
	todo();
}

static void
op_bpl()
{
	if (reg.P.N == 0)
		reg.PC = cpu_addr;
}

static void
op_clc()
{
	todo();
}

static void
op_jsr()
{
	todo();
}

static void
op_and()
{
	todo();
}

static void
op_rla()
{
	todo();
}

static void
op_bit()
{
	todo();
}

static void
op_rol()
{
	todo();
}

static void
op_plp()
{
	todo();
}

static void
op_bmi()
{
	todo();
}

static void
op_sec()
{
	todo();
}

static void
op_rti()
{
	todo();
}

static void
op_eor()
{
	todo();
}

static void
op_sre()
{
	todo();
}

static void
op_lsr()
{
	flag_carry(cpu_arg % 2);
	flag_zero(cpu_arg >> 1);
	flag_neg(cpu_arg >> 1);

	ram_setb(cpu_addr, cpu_arg >> 1);
}

static void
op_pha()
{
	todo();
}

static void
op_alr()
{
	todo();
}

static void
op_jmp()
{
	reg.PC = cpu_addr;
}

static void
op_bvc()
{
	todo();
}

static void
op_cli()
{
	todo();
}

static void
op_rts()
{
	todo();
}

static void
op_adc()
{
	todo();
}

static void
op_rra()
{
	todo();
}

static void
op_ror()
{
	todo();
}

static void
op_pla()
{
	todo();
}

static void
op_arr()
{
	todo();
}

static void
op_bvs()
{
	todo();
}

static void
op_sei()
{
	todo();
}

static void
op_sta()
{
	ram_setb(cpu_addr, reg.A);
}

static void
op_sax()
{
	todo();
}

static void
op_stx()
{
	todo();
}

static void
op_sty()
{
	todo();
}

static void
op_dey()
{
	todo();
}

static void
op_txa()
{
	todo();
}

static void
op_xaa()
{
	todo();
}

static void
op_bcc()
{
	todo();
}

static void
op_tya()
{
	todo();
}

static void
op_ahx()
{
	todo();
}

static void
op_txs()
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
op_ldy()
{
	todo();
}

static void
op_lda()
{
	reg.A = cpu_arg;

	flag_neg(reg.A);
	flag_zero(reg.A);
}

static void
op_ldx()
{
	todo();
}

static void
op_lax()
{
	todo();
}

static void
op_tay()
{
	todo();
}

static void
op_tax()
{
	todo();
}

static void
op_bcs()
{
	todo();
}

static void
op_clv()
{
	todo();
}

static void
op_tsx()
{
	todo();
}

static void
op_las()
{
	todo();
}

static void
op_cpy()
{
	todo();
}

static void
op_cmp()
{
	todo();
}

static void
op_dcp()
{
	todo();
}

static void
op_dec()
{
	todo();
}

static void
op_iny()
{
	todo();
}

static void
op_dex()
{
	todo();
}

static void
op_axs()
{
	todo();
}

static void
op_bne()
{
	todo();
}

static void
op_cld()
{
	todo();
}

static void
op_cpx()
{
	todo();
}

static void
op_sbc()
{
	todo();
}

static void
op_isc()
{
	todo();
}

static void
op_inc()
{
	sbyte n;

	n = cpu_arg + 1;

	flag_neg(n);
	flag_zero(n);

	ram_setb(cpu_addr, n);
}

static void
op_inx()
{
	todo();
}

static void
op_beq()
{
	todo();
}

static void
op_sed()
{
	todo();
}

#undef todo

#define OP(hex, comm, md, cycl) [(hex)] = {	\
		.cmd	= (comm),		\
		.mode	= (md),			\
		.cycles	= (cycl),		\
		.name	= #comm,		\
	},

struct opcode ops[256] =
{
	OP(0x00,	op_brk,		NULL,		7)
	OP(0x01,	op_ora,		addr_mode_izx,	6)
	OP(0x03,	op_slo,		addr_mode_izx,	8)
	OP(0x04,	op_nop,		NULL,		3)
	OP(0x05,	op_ora,		addr_mode_zp,	3)
	OP(0x06,	op_asl,		addr_mode_zp,	5)
	OP(0x07,	op_slo,		addr_mode_zp,	5)
	OP(0x08,	op_php,		NULL,		3)
	OP(0x09,	op_ora,		addr_mode_imm,	2)
	OP(0x0A,	op_asl,		NULL,		2)
	OP(0x0B,	op_anc,		addr_mode_imm,	2)
	OP(0x0C,	op_nop,		NULL,		4)
	OP(0x0D,	op_ora,		addr_mode_abs,	4)
	OP(0x0E,	op_asl,		addr_mode_abs,	6)
	OP(0x0F,	op_slo,		addr_mode_abs,	6)
	OP(0x10,	op_bpl,		addr_mode_rel,	3)
	OP(0x11,	op_ora,		addr_mode_izy,	5)
	OP(0x13,	op_slo,		addr_mode_izy,	8)
	OP(0x14,	op_nop,		NULL,		4)
	OP(0x15,	op_ora,		addr_mode_zpx,	4)
	OP(0x16,	op_asl,		addr_mode_zpx,	6)
	OP(0x17,	op_slo,		addr_mode_zpx,	6)
	OP(0x18,	op_clc,		NULL,		2)
	OP(0x19,	op_ora,		addr_mode_aby,	4)
	OP(0x1A,	op_nop,		NULL,		2)
	OP(0x1B,	op_slo,		addr_mode_aby,	7)
	OP(0x1C,	op_nop,		NULL,		4)
	OP(0x1D,	op_ora,		addr_mode_abx,	4)
	OP(0x1E,	op_asl,		addr_mode_abx,	7)
	OP(0x1F,	op_slo,		addr_mode_abx,	7)
	OP(0x20,	op_jsr,		addr_mode_abs,	6)
	OP(0x21,	op_and,		addr_mode_izx,	6)
	OP(0x23,	op_rla,		addr_mode_izx,	8)
	OP(0x24,	op_bit,		addr_mode_zp,	3)
	OP(0x25,	op_and,		addr_mode_zp,	3)
	OP(0x26,	op_rol,		addr_mode_zp,	5)
	OP(0x27,	op_rla,		addr_mode_zp,	5)
	OP(0x28,	op_plp,		NULL,		4)
	OP(0x29,	op_and,		addr_mode_imm,	2)
	OP(0x2A,	op_rol,		NULL,		2)
	OP(0x2C,	op_bit,		addr_mode_abs,	4)
	OP(0x2D,	op_and,		addr_mode_abs,	4)
	OP(0x2E,	op_rol,		addr_mode_abs,	6)
	OP(0x2F,	op_rla,		addr_mode_abs,	6)
	OP(0x30,	op_bmi,		addr_mode_rel,	2)
	OP(0x31,	op_and,		addr_mode_izy,	5)
	OP(0x33,	op_rla,		addr_mode_izy,	8)
	OP(0x35,	op_and,		addr_mode_zpx,	4)
	OP(0x36,	op_rol,		addr_mode_zpx,	6)
	OP(0x37,	op_rla,		addr_mode_zpx,	6)
	OP(0x38,	op_sec,		NULL,		2)
	OP(0x39,	op_and,		addr_mode_aby,	4)
	OP(0x3B,	op_rla,		addr_mode_aby,	7)
	OP(0x3D,	op_and,		addr_mode_abx,	4)
	OP(0x3E,	op_rol,		addr_mode_abx,	7)
	OP(0x3F,	op_rla,		addr_mode_abx,	7)
	OP(0x40,	op_rti,		NULL,		6)
	OP(0x41,	op_eor,		addr_mode_izx,	6)
	OP(0x43,	op_sre,		addr_mode_izx,	8)
	OP(0x45,	op_eor,		addr_mode_zp,	3)
	OP(0x46,	op_lsr,		addr_mode_zp,	5)
	OP(0x47,	op_sre,		addr_mode_zp,	5)
	OP(0x48,	op_pha,		NULL,		3)
	OP(0x49,	op_eor,		addr_mode_imm,	2)
	OP(0x4A,	op_lsr,		NULL,		2)
	OP(0x4B,	op_alr,		addr_mode_imm,	2)
	OP(0x4C,	op_jmp,		addr_mode_abs,	3)
	OP(0x4D,	op_eor,		addr_mode_abs,	4)
	OP(0x4E,	op_lsr,		addr_mode_abs,	6)
	OP(0x4F,	op_sre,		addr_mode_abs,	6)
	OP(0x50,	op_bvc,		addr_mode_rel,	3)
	OP(0x51,	op_eor,		addr_mode_izy,	5)
	OP(0x53,	op_sre,		addr_mode_izy,	8)
	OP(0x55,	op_eor,		addr_mode_zpx,	4)
	OP(0x56,	op_lsr,		addr_mode_zpx,	6)
	OP(0x57,	op_sre,		addr_mode_zpx,	6)
	OP(0x58,	op_cli,		NULL,		2)
	OP(0x59,	op_eor,		addr_mode_aby,	4)
	OP(0x5B,	op_sre,		addr_mode_aby,	7)
	OP(0x5D,	op_eor,		addr_mode_abx,	4)
	OP(0x5E,	op_lsr,		addr_mode_abx,	7)
	OP(0x5F,	op_sre,		addr_mode_abx,	7)
	OP(0x60,	op_rts,		NULL,		6)
	OP(0x61,	op_adc,		addr_mode_izx,	6)
	OP(0x63,	op_rra,		addr_mode_izx,	8)
	OP(0x65,	op_adc,		addr_mode_zp,	3)
	OP(0x66,	op_ror,		addr_mode_zp,	5)
	OP(0x67,	op_rra,		addr_mode_zp,	5)
	OP(0x68,	op_pla,		NULL,		4)
	OP(0x69,	op_adc,		addr_mode_imm,	2)
	OP(0x6A,	op_ror,		NULL,		2)
	OP(0x6B,	op_arr,		addr_mode_imm,	2)
	OP(0x6C,	op_jmp,		addr_mode_ind,	5)
	OP(0x6D,	op_adc,		addr_mode_abs,	4)
	OP(0x6E,	op_ror,		addr_mode_abs,	6)
	OP(0x6F,	op_rra,		addr_mode_abs,	6)
	OP(0x70,	op_bvs,		addr_mode_rel,	2)
	OP(0x71,	op_adc,		addr_mode_izy,	5)
	OP(0x73,	op_rra,		addr_mode_izy,	8)
	OP(0x75,	op_adc,		addr_mode_zpx,	4)
	OP(0x76,	op_ror,		addr_mode_zpx,	6)
	OP(0x77,	op_rra,		addr_mode_zpx,	6)
	OP(0x78,	op_sei,		NULL,		2)
	OP(0x79,	op_adc,		addr_mode_aby,	4)
	OP(0x7B,	op_rra,		addr_mode_aby,	7)
	OP(0x7D,	op_adc,		addr_mode_abx,	4)
	OP(0x7E,	op_ror,		addr_mode_abx,	7)
	OP(0x7F,	op_rra,		addr_mode_abx,	7)
	OP(0x80,	op_nop,		NULL,		2)
	OP(0x81,	op_sta,		addr_mode_izx,	6)
	OP(0x83,	op_sax,		addr_mode_izx,	6)
	OP(0x84,	op_sty,		addr_mode_zp,	3)
	OP(0x85,	op_sta,		addr_mode_zp,	3)
	OP(0x86,	op_stx,		addr_mode_zp,	3)
	OP(0x87,	op_sax,		addr_mode_zp,	3)
	OP(0x88,	op_dey,		NULL,		2)
	OP(0x8A,	op_txa,		NULL,		2)
	OP(0x8B,	op_xaa,		addr_mode_imm,	2)
	OP(0x8C,	op_sty,		addr_mode_abs,	4)
	OP(0x8D,	op_sta,		addr_mode_abs,	4)
	OP(0x8E,	op_stx,		addr_mode_abs,	4)
	OP(0x8F,	op_sax,		addr_mode_abs,	4)
	OP(0x90,	op_bcc,		addr_mode_rel,	3)
	OP(0x91,	op_sta,		addr_mode_izy,	6)
	OP(0x93,	op_ahx,		addr_mode_izy,	6)
	OP(0x94,	op_sty,		addr_mode_zpx,	4)
	OP(0x95,	op_sta,		addr_mode_zpx,	4)
	OP(0x96,	op_stx,		addr_mode_zpy,	4)
	OP(0x97,	op_sax,		addr_mode_zpy,	4)
	OP(0x98,	op_tya,		NULL,		2)
	OP(0x99,	op_sta,		addr_mode_aby,	5)
	OP(0x9A,	op_txs,		NULL,		2)
	OP(0x9B,	op_tas,		addr_mode_aby,	5)
	OP(0x9C,	op_shy,		addr_mode_abx,	5)
	OP(0x9D,	op_sta,		addr_mode_abx,	5)
	OP(0x9E,	op_shx,		addr_mode_aby,	5)
	OP(0x9F,	op_ahx,		addr_mode_aby,	5)
	OP(0xA0,	op_ldy,		addr_mode_imm,	2)
	OP(0xA1,	op_lda,		addr_mode_izx,	6)
	OP(0xA2,	op_ldx,		addr_mode_imm,	2)
	OP(0xA3,	op_lax,		addr_mode_izx,	6)
	OP(0xA4,	op_ldy,		addr_mode_zp,	3)
	OP(0xA5,	op_lda,		addr_mode_zp,	3)
	OP(0xA6,	op_ldx,		addr_mode_zp,	3)
	OP(0xA7,	op_lax,		addr_mode_zp,	3)
	OP(0xA8,	op_tay,		NULL,		2)
	OP(0xA9,	op_lda,		addr_mode_imm,	2)
	OP(0xAA,	op_tax,		NULL,		2)
	OP(0xAB,	op_lax,		addr_mode_imm,	2)
	OP(0xAC,	op_ldy,		addr_mode_abs,	4)
	OP(0xAD,	op_lda,		addr_mode_abs,	4)
	OP(0xAE,	op_ldx,		addr_mode_abs,	4)
	OP(0xAF,	op_lax,		addr_mode_abs,	4)
	OP(0xB0,	op_bcs,		addr_mode_rel,	2)
	OP(0xB1,	op_lda,		addr_mode_izy,	5)
	OP(0xB3,	op_lax,		addr_mode_izy,	5)
	OP(0xB4,	op_ldy,		addr_mode_zpx,	4)
	OP(0xB5,	op_lda,		addr_mode_zpx,	4)
	OP(0xB6,	op_ldx,		addr_mode_zpy,	4)
	OP(0xB7,	op_lax,		addr_mode_zpy,	4)
	OP(0xB8,	op_clv,		NULL,		2)
	OP(0xB9,	op_lda,		addr_mode_aby,	4)
	OP(0xBA,	op_tsx,		NULL,		2)
	OP(0xBB,	op_las,		addr_mode_aby,	4)
	OP(0xBC,	op_ldy,		addr_mode_abx,	4)
	OP(0xBD,	op_lda,		addr_mode_abx,	4)
	OP(0xBE,	op_ldx,		addr_mode_aby,	4)
	OP(0xBF,	op_lax,		addr_mode_aby,	4)
	OP(0xC0,	op_cpy,		addr_mode_imm,	2)
	OP(0xC1,	op_cmp,		addr_mode_izx,	6)
	OP(0xC3,	op_dcp,		addr_mode_izx,	8)
	OP(0xC4,	op_cpy,		addr_mode_zp,	3)
	OP(0xC5,	op_cmp,		addr_mode_zp,	3)
	OP(0xC6,	op_dec,		addr_mode_zp,	5)
	OP(0xC7,	op_dcp,		addr_mode_zp,	5)
	OP(0xC8,	op_iny,		NULL,		2)
	OP(0xC9,	op_cmp,		addr_mode_imm,	2)
	OP(0xCA,	op_dex,		NULL,		2)
	OP(0xCB,	op_axs,		addr_mode_imm,	2)
	OP(0xCC,	op_cpy,		addr_mode_abs,	4)
	OP(0xCD,	op_cmp,		addr_mode_abs,	4)
	OP(0xCE,	op_dec,		addr_mode_abs,	6)
	OP(0xCF,	op_dcp,		addr_mode_abs,	6)
	OP(0xD0,	op_bne,		addr_mode_rel,	3)
	OP(0xD1,	op_cmp,		addr_mode_izy,	5)
	OP(0xD3,	op_dcp,		addr_mode_izy,	8)
	OP(0xD5,	op_cmp,		addr_mode_zpx,	4)
	OP(0xD6,	op_dec,		addr_mode_zpx,	6)
	OP(0xD7,	op_dcp,		addr_mode_zpx,	6)
	OP(0xD8,	op_cld,		NULL,		2)
	OP(0xD9,	op_cmp,		addr_mode_aby,	4)
	OP(0xDB,	op_dcp,		addr_mode_aby,	7)
	OP(0xDD,	op_cmp,		addr_mode_abx,	4)
	OP(0xDE,	op_dec,		addr_mode_abx,	7)
	OP(0xDF,	op_dcp,		addr_mode_abx,	7)
	OP(0xE0,	op_cpx,		addr_mode_imm,	2)
	OP(0xE1,	op_sbc,		addr_mode_izx,	6)
	OP(0xE3,	op_isc,		addr_mode_izx,	8)
	OP(0xE4,	op_cpx,		addr_mode_zp,	3)
	OP(0xE5,	op_sbc,		addr_mode_zp,	3)
	OP(0xE6,	op_inc,		addr_mode_zp,	5)
	OP(0xE7,	op_isc,		addr_mode_zp,	5)
	OP(0xE8,	op_inx,		NULL,		2)
	OP(0xE9,	op_sbc,		addr_mode_imm,	2)
	OP(0xEC,	op_cpx,		addr_mode_abs,	4)
	OP(0xED,	op_sbc,		addr_mode_abs,	4)
	OP(0xEE,	op_inc,		addr_mode_abs,	6)
	OP(0xEF,	op_isc,		addr_mode_abs,	6)
	OP(0xF0,	op_beq,		addr_mode_rel,	2)
	OP(0xF1,	op_sbc,		addr_mode_izy,	5)
	OP(0xF3,	op_isc,		addr_mode_izy,	8)
	OP(0xF5,	op_sbc,		addr_mode_zpx,	4)
	OP(0xF6,	op_inc,		addr_mode_zpx,	6)
	OP(0xF7,	op_isc,		addr_mode_zpx,	6)
	OP(0xF8,	op_sed,		NULL,		2)
	OP(0xF9,	op_sbc,		addr_mode_aby,	4)
	OP(0xFB,	op_isc,		addr_mode_aby,	7)
	OP(0xFD,	op_sbc,		addr_mode_abx,	4)
	OP(0xFE,	op_inc,		addr_mode_abx,	7)
	OP(0xFF,	op_isc,		addr_mode_abx,	7)
};

#undef OP

