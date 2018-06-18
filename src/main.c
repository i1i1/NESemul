#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "cpu.h"
#include "ram.h"


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
	printf("\tCHR-RoM - %d * 8 Kb\n", hdr->chr_rom_num);
	printf("\tPRG-RAM - %d * 8 Kb\n", hdr->prg_ram_num);
	printf("\tPRG-RAM - %d * 8 Kb\n", hdr->prg_ram_num);
	printf("\tMapper - %d\n", mapper);
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

void
load_chr(FILE *fp, struct ines_header *hdr)
{
	;
}

void
load_prg_rom(FILE *fp, byte prg_max)
{
	unsigned int i, j;

	prg = malloc(sizeof(void *) * prg_max);

	if (!prg)
		die("Not enough memory!");

	for (i = 0; i < prg_max; i++) {
		prg[i] = malloc(sizeof(struct prg_rom));

		if (!prg[i])
			die("Not enough memory!");

		for (j = 0; j < sizeof(struct prg_rom); j++)
			prg[i]->arr[j] = xgetb(fp);
	}
}

void
load_rom(FILE *fp)
{
	load_header(fp, &hdr);
	load_prg_rom(fp, hdr.prg_rom_num);
	load_chr(fp, &hdr);
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
	cpu_init();

	return 0;
}

