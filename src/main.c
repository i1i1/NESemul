#include <SDL2/SDL.h>

#include <stdlib.h>
#include <string.h>

#include "common.h"

#include "window.h"
#include "cpu.h"
#include "ppu.h"
#include "ram.h"


struct ines_header hdr;

byte mapper;


void
die(const char *msg)
{
	fprintf(stderr, "Error occured:\n\t%s\n", msg);
	exit(0);
}

void
printhdr(struct ines_header *hdr)
{
	fprintf(stderr, "\nInfo:\n");
	fprintf(stderr, "\tMapper   - %d\n", mapper);
	fprintf(stderr, "\tPRG-ROM  - %d * 16 Kb\n", hdr->prg_rom_num);
	fprintf(stderr, "\tCHR-ROM  - %d * 8 Kb\n", hdr->chr_rom_num);
	fprintf(stderr, "\tPRG-RAM  - %d * 8 Kb\n", hdr->prg_ram_num);
	fprintf(stderr, "\tVertical - %d\n", ppu.vmap);

	(void) hdr;
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

	mapper = ((hdr->flag7 >> 4) << 4) | (hdr->flag6 >> 4);
	ppu.vmap = hdr->flag6 & 1;
	printhdr(hdr);
}

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
load_chr_rom(FILE *fp, byte chr_rom_max)
{
	unsigned int i;
	int ret;

	chr_rom.bank = malloc(sizeof(void *) * chr_rom_max);

	if (!chr_rom.bank)
		die("Not enough memory!");

	for (i = 0; i < chr_rom_max; i++) {
		chr_rom.bank[i] = malloc(PRG_ROM_BANK_SZ);

		if (!chr_rom.bank[i])
			die("Not enough memory!");

		ret = fread(chr_rom.bank[i], 1, 0x2000, fp);

		if (ret == 0 && ferror(fp))
			die("Wrong format of file!");
	}

	chr_rom.n = chr_rom_max;
}

void
load_rom(FILE *fp)
{
	load_header(fp, &hdr);
	load_prg_rom(fp, hdr.prg_rom_num);
	load_chr_rom(fp, hdr.chr_rom_num);
}

void
main_loop()
{
	/* All time in ms */
	uint32_t curtm, dsttm;
	int cpu_cycles, ppu_cycles;
	int i;

	cpu_cycles = (int)CPU_CYCLES_P_SCANLINE;
	ppu_cycles = 1;

	dsttm = 1000/WINDOW_FRAME_PS;

	for (;;) {
		curtm = SDL_GetTicks();

		for (i = 0; i < 262; i++) {
			cpu_run_cycles(cpu_cycles);
			ppu_run_cycles(ppu_cycles);
		}

		if (window_event_exit())
			window_deinit();

		curtm = SDL_GetTicks() - curtm;

		if (curtm < dsttm)
			SDL_Delay(dsttm - curtm);

		fprintf(stderr, "time: %3ld\n", (long)curtm - (long)dsttm);
	}
}

int
main(int argc, char **argv)
{
	FILE *fp;

	if (argc < 2)
		die("Give me some file!");

	fp = fopen(argv[1], "r");

	if (fp == NULL)
		die("Cant open file!");

	load_rom(fp);
	fclose(fp);

	ram_init();
	cpu_init();
	ppu_init();

	window_init();

	main_loop();

	return 0;
}

