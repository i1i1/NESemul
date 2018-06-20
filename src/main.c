#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

#include "cpu.h"
#include "ppu.h"

#include "ram.h"
#include "opcodes.h"


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

struct ines_header hdr;

byte mapper;


void
die(const char *msg)
{
	printf("Error occured:\n\t%s\n", msg);
	exit(1);
}

void
printhdr(struct ines_header *hdr)
{
	printf("\nInfo:\n");
	printf("\tPRG-ROM - %d * 16 Kb\n", hdr->prg_rom_num);
	printf("\tCHR-ROM - %d * 8 Kb\n", hdr->chr_rom_num);
	printf("\tPRG-RAM - %d * 8 Kb\n", hdr->prg_ram_num);
	printf("\tPRG-RAM - %d * 8 Kb\n", hdr->prg_ram_num);
}

void
load_header(FILE *fp, struct ines_header *hdr)
{
	int ret;

	ret = fread(hdr, 1, sizeof(struct ines_header), fp);

	if (ret == 0 && ferror(fp))
		die("Wrong format of file!");

	if (strncmp((char *)hdr->magick, "NES\x1A", 4) != 0)
		die("Wrong magick number!");

	mapper = ((hdr->flag7 >> 4) << 4)| (hdr->flag6 >> 4);
	printhdr(hdr);
}

/* TODO! */
#if 0
void
load_chr(FILE *fp, struct ines_header *hdr)
{
	;
}
#endif

void
load_prg_rom(FILE *fp, byte prg_rom_max)
{
	unsigned int i;
	int ret;

	prg_rom.bank = malloc(sizeof(void *) * prg_rom_max);

	if (!prg_rom.bank)
		die("Not enough memory!");

	for (i = 0; i < prg_rom_max; i++) {
		prg_rom.bank[i] = malloc(PRG_ROM_BANK_SZ);

		if (!prg_rom.bank[i])
			die("Not enough memory!");

		ret = fread(prg_rom.bank[i], 1, PRG_ROM_BANK_SZ, fp);

		if (ret == 0 && ferror(fp))
			die("Wrong format of file!");
	}

	prg_rom.n = prg_rom_max;
}

void
load_rom(FILE *fp)
{
	load_header(fp, &hdr);
	load_prg_rom(fp, hdr.prg_rom_num);
//	load_chr(fp, &hdr);
}

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

	if (ops[op].cmd == NULL) {
		printf("Unknown opcode %02x!\n", op);
		die("");
	}

	printf("%s", ops[op].cname);
	if (ops[op].mode)
		printf(" %s", ops[op].mname);

	printf("\t\t(A=%02x;\tX=%02x;\tY=%02x;\tSP=%02x;\tP=%02x)\n",
	       (byte)reg.A, (byte)reg.X, (byte)reg.Y, (byte)reg.SP, reg.P);
}

void
main_loop()
{
	byte op;

	for (;;) {
		printinfo();

		op = ram_getb(reg.PC);
		reg.PC++;

		if (ops[op].mode)
			ops[op].mode();

		if (ops[op].cmd == NULL) {
			printf("Unknown opcode %02x!\n", op);
			die("");
		}

		ops[op].cmd();
	}
}

int
main(int argc, char **argv)
{
	FILE *fp;

	if (argc < 2)
		die("Give me some file!");

	if ((fp = fopen(argv[1], "r")) == NULL)
		die("Cant open file!");

	load_rom(fp);
	ram_init();
	cpu_init();
	ppu_init();

	main_loop();

	return 0;
}

