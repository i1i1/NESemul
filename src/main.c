#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "cpu.h"
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

byte
xgetb(FILE *fp)
{
	int c;

	c = fgetc(fp);

	if (c == EOF)
		die("Wrong format of file!");

	return (char)c;
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
	byte *p;
	unsigned int i;

	for (p = (void *)hdr, i = 0; i < sizeof(struct ines_header); i++)
		p[i] = xgetb(fp);

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
	unsigned int i, j;

	prg_rom.bank = malloc(sizeof(void *) * prg_rom_max);

	if (!prg_rom.bank)
		die("Not enough memory!");

	for (i = 0; i < prg_rom_max; i++) {
		prg_rom.bank[i] = malloc(PRG_ROM_BANK_SZ);

		if (!prg_rom.bank[i])
			die("Not enough memory!");

		for (j = 0; j < PRG_ROM_BANK_SZ; j++)
			prg_rom.bank[i][j] = xgetb(fp);
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
main_loop()
{
	byte op;

	for (;;) {
		printf("%04x:\t", reg.PC);

		op = ram_getb(reg.PC);
		reg.PC++;

		printf("%02x\t%s\n", op, ops[op].name + 3);

		if (ops[op].mode)
			ops[op].mode();

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

	main_loop();

	return 0;
}

