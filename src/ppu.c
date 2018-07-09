#include "common.h"
#include "ram.h"
#include "cpu.h"
#include "ppu.h"
#include "window.h"

struct ppu ppu;

const struct color ppu_palette[0x40] = {
#define CLR(idx, red, green, blue) [(idx)] = {	\
		.r = (red), .g = (green), .b = blue },

	CLR(0x00, 84, 84, 84)
	CLR(0x01, 0, 30, 116)
	CLR(0x02, 8, 16, 144)
	CLR(0x03, 48, 0, 136)
	CLR(0x04, 68, 0, 100)
	CLR(0x05, 92, 0, 48)
	CLR(0x06, 84, 4, 0)
	CLR(0x07, 60, 24, 0)
	CLR(0x08, 32, 42, 0)
	CLR(0x09, 8, 58, 0)
	CLR(0x0a, 0, 64, 0)
	CLR(0x0b, 0, 60, 0)
	CLR(0x0c, 0, 50, 60)
	CLR(0x0d, 0, 0, 0)
	CLR(0x0e, 0, 0, 0)
	CLR(0x0f, 0, 0, 0)

	CLR(0x10, 152, 150, 152)
	CLR(0x11, 8, 76, 196)
	CLR(0x12, 48, 50, 236)
	CLR(0x13, 92, 30, 228)
	CLR(0x14, 136, 20, 176)
	CLR(0x15, 160, 20, 100)
	CLR(0x16, 152, 34, 32)
	CLR(0x17, 120, 60, 0)
	CLR(0x18, 84, 90, 0)
	CLR(0x19, 40, 114, 0)
	CLR(0x1a, 8, 124, 0)
	CLR(0x1b, 0, 118, 40)
	CLR(0x1c, 0, 102, 120)
	CLR(0x1d, 0, 0, 0)
	CLR(0x1e, 0, 0, 0)
	CLR(0x1f, 0, 0, 0)

	CLR(0x20, 236, 238, 236)
	CLR(0x21, 76, 154, 236)
	CLR(0x22, 120, 124, 236)
	CLR(0x23, 176, 98, 236)
	CLR(0x24, 228, 84, 236)
	CLR(0x25, 236, 88, 180)
	CLR(0x26, 236, 106, 100)
	CLR(0x27, 212, 136, 32)
	CLR(0x28, 160, 170, 0)
	CLR(0x29, 116, 196, 0)
	CLR(0x2a, 76, 208, 32)
	CLR(0x2b, 56, 204, 108)
	CLR(0x2c, 56, 180, 204)
	CLR(0x2d, 60, 60, 60)
	CLR(0x2e, 0, 0, 0)
	CLR(0x2f, 0, 0, 0)

	CLR(0x30, 236, 238, 236)
	CLR(0x31, 76, 154, 236)
	CLR(0x32, 120, 124, 236)
	CLR(0x33, 176, 98, 236)
	CLR(0x34, 228, 84, 236)
	CLR(0x35, 236, 88, 180)
	CLR(0x36, 236, 106, 100)
	CLR(0x37, 212, 136, 32)
	CLR(0x38, 160, 170, 0)
	CLR(0x39, 116, 196, 0)
	CLR(0x3a, 76, 208, 32)
	CLR(0x3b, 56, 204, 108)
	CLR(0x3c, 56, 180, 204)
	CLR(0x3d, 60, 60, 60)
	CLR(0x3e, 60, 60, 60)
	CLR(0x3f, 60, 60, 60)
};
#undef CLR


byte
ppu_reg_get(word addr)
{
	byte ret;

	if (addr < 0x4000)
		addr = 0x2000 + (addr - 0x2000) % 8;

	printf("Accessing %04x register\n", addr);

	switch (addr) {
	case PPUSTATUS:
		ppu.scroll = ppu.addr = 0;
		ret = ppu.PPUSTATUS;

		/* In vblank = False */
		ppu.PPUSTATUS &= ~(1 << 7);
		/* In Sprite-0 hit = False */
		ppu.PPUSTATUS &= ~(1 << 6);

		return ret;
	case PPUDATA:
		ret = ppu_getb(ppu.PPUADDR);
		ppu.PPUADDR += (ppu.PPUCTRL & (1 << 2)) ? 32 : 1;

		return ret;
	case OAMDATA:
		return ppu.spr_ram[ppu.OAMADDR++];
	/* Joystick 1 */
	case 0x4016:
		return joy1_read_state();
	/* If some WO register or invalide register */
	default:
		//todo();
		return 0;
	}
}

void
ppu_load_spr_ram(word addr)
{
	int i;

	printf("Loading spr_ram addr = %04x!\n", addr);
	for (i = 0; i < 0x100; i++) {
		printf("OAM $%02x 0x%02x\n", i, ram_getb(addr + i));
	}

	for (i = 0; i < 0x100; i++)
		ppu.spr_ram[i] = ram_getb(addr + i);
}

void
ppu_setb(word a, byte b)
{
	a %= 0x4000;

	/* Mirroring */
	if (0x3f20 <= a)
		a = 0x3f20 + a % 0x20;

	/* Mirroring */
	if (0x3000 <= a && a < 0x3f00)
		a = 0x2000 + a % 0x1000;

	/* If not a nametable */
	if (!(0x2000 <= a && a < 0x3000))
		goto setting;

	/* Some nametable mirroring */
	if (ppu.vmap)
		a = 0x2000 + a % 0x800;
	else
		if (0x2000 <= a && a < 0x2800)
			a = 0x2000 + a % 0x400;
		else
			a = 0x2800 + a % 0x400;

setting:
	ppu.ram[a] = b;
}

byte
ppu_getb(word a)
{
	a %= 0x4000;

	if (a < 0x2000) {
		if (chr_rom.n)
			return chr_rom.bank[chr_rom.cur][a];
		else
			return ppu.ram[a];
	}

	/* Mirroring */
	if (0x3f20 <= a)
		a = 0x3f20 + a % 0x20;

	/* Mirroring */
	if (0x3000 <= a && a < 0x3f00)
		a = 0x2000 + a % 0x1000;

	/* If not a nametable */
	if (a >= 0x3000)
		return ppu.ram[a];

	/* Some nametable mirroring */
	if (ppu.vmap)
		a = 0x2000 + a % 0x800;
	else
		if (0x2000 <= a && a < 0x2800)
			a = 0x2000 + a % 0x400;
		else
			a = 0x2800 + a % 0x400;

	return ppu.ram[a];
}

void
ppu_reg_set(word addr, byte b)
{
	/* Some APU registers. Ignored for now */
	if (addr >= 0x4000 && (addr != OAMDMA) && (addr != 0x4016))
		return;

	/* Some mirroring */
	if (0x2000 <= addr && addr < 0x4000)
		addr = 0x2000 + addr % 8;

	printf("Setting reg %04x to %02x\n", addr, b);

	switch (addr) {
	case OAMDMA:
		ppu_load_spr_ram(b * 0x100);
		break;
	case PPUSCROLL:
		if (ppu.scroll)
			ppu.PPUSCROLL_X = b;
		else
			ppu.PPUSCROLL_Y = b;

		ppu.scroll++;
		ppu.scroll %= 2;
		break;
	case PPUCTRL:
		if (ppu.ready)
			ppu.PPUCTRL = b;
		break;
	case PPUMASK:
		if (ppu.ready)
			ppu.PPUMASK = b;
		break;
	case PPUADDR:
		if (!ppu.ready)
			break;

		ppu.PPUADDR <<= ppu.addr * 8;
		ppu.PPUADDR |= b;

		ppu.addr++;
		ppu.addr %= 2;
		break;
	case PPUDATA:
		ppu_setb(ppu.PPUADDR, b);
		ppu.PPUADDR += (ppu.PPUCTRL & (1 << 2)) ? 32 : 1;
		break;
	case OAMADDR:
		ppu.OAMADDR = b;
		break;
	case OAMDATA:
		ppu.spr_ram[(byte)(ppu.OAMADDR - 1)] = b;
		break;
	/* Joystick 1 */
	case 0x4016:
		joy1_write(b);
		break;
	default:
		todo();
	}
}

byte
ppu_is_reg_r(word addr)
{
	/* Some mirroring */
	if (0x2000 <= addr && addr < 0x4000)
		addr = 0x2000 + addr % 8;

	switch (addr) {
	case PPUSTATUS:
	case OAMDATA:
	case PPUDATA:
		return 1;
	default:
		return 0;
	}
}

word
ppu_patt_tbl()
{
	return (ppu.PPUCTRL & (1 << 3)) ? 0x1000 : 0x0000;
}

word
ppu_nametbl()
{
	return 0x2000 + (ppu.PPUCTRL & 3) * 0x400;
}

void
ppu_draw_bg_line(byte mirror)
{
	#if 0
	int tx, ty, ti, pal_idx;
	word name, attr, patt, pal, tile;

	name = ppu_nametbl();
	attr = name + 0x3C0;

	patt = ppu_patt_tbl();

	ty = ppu.scanline / 8;

	for (tx = ppu.PPUMASK >> 1; tx < 32; tx++) {
		if (tx * 8 - ppu.PPUSCROLL_X + (mirror ? 256 : 0) > 256)
			continue;

		ti = ppu_getb(name + 32 * ty + tx + (mirror ? 0x400 : 0));
		tile = patt + 16 * ti;
	}

	for (i = 0; i < 256; i += 8) {
		pal_idx = ppu_getb(attr + i / 32 +
				   (ppu.scanline >> 5) * 8);

		if (ppu.scanline % 32 >= 16)
			pal_idx >>= 4;
		if ((i / 8) % 32 >= 16)
			pal_idx >>= 2;
		pal_idx &= 3;

		pal = 0x3f00 + pal_idx * 4;

		;
	}
	#endif
}

void
ppu_draw_sprites_line()
{
	int i, j;
	byte x, y;
	byte sprh, clr, hflip, vflip;
	word patt, tile, pal;

	patt = ppu_patt_tbl();

	sprh = (ppu.PPUCTRL & (1 << 5)) ? 16 : 8;

	for (i = 0; i < 0x100; i += 4) {
		tile = patt + 16 * ppu.spr_ram[i + 1];
		x = ppu.spr_ram[i + 3];
		y = ppu.spr_ram[i];
		pal = 0x3f10 + (ppu.spr_ram[i + 2] & 3) * 4;
		hflip = ppu.spr_ram[i + 2] & (1 << 6);
		vflip = ppu.spr_ram[i + 2] & (1 << 7);

		if (y > ppu.scanline || y + sprh <= ppu.scanline)
			continue;

		printf("Sprite %d!\n", i);
		printf("Scanline %d!\n", ppu.scanline);

#define PRINTTOBIN(var)	do {						\
			int _i;						\
			for (_i = 7; _i >= 0; _i--)			\
				printf("%d", (var) & (1 << _i) ? 1 : 0);\
		} while(0)

		for (j = 0; j < 0x10; j++) {
			printf("$%04x = 0x%02x -- ", tile + j,
			       ppu_getb(tile + j));
			PRINTTOBIN(ppu_getb(tile + j));
			printf("\n");
		}

		printf("\n");

		for (j = 0; j < 4; j++) {
			printf("$%02x = 0x%02x  --  ", i + j,
			       ppu.spr_ram[i + j]);
			PRINTTOBIN(ppu.spr_ram[i + j]);
			printf("\n");
		}

		printf("\n");

		if (ppu.spr_ram[i + 2] & 0xc0)
			todo();

#define SETCLR(clr, tile, x, y) do {						\
		clr = (ppu_getb((tile) + (y) + 8) >> (7 - (x))) & 1; 		\
		clr = (clr << 1) | ((ppu_getb((tile) + (y)) >> (7 - (x))) & 1); \
	} while(0)

		for (j = 0; j < 8; j++) {
			SETCLR(clr, tile, j, ppu.scanline - y);

			if (clr) {
				bg.arr[ppu.scanline + 1][x + j] = ppu_getb(pal + clr);
				printf("clr %d!\n", bg.arr[ppu.scanline + 1][x + j]);
				printf("y = $%x, x = $%x!\n", ppu.scanline + 1, x + j);
			}
		}
	}
	//todo();
}

void
ppu_draw_screen()
{
	window_set_to_layer(&bg);
	window_flush();

	window_layer_clear(&bg, ppu_getb(0x3f00));
	window_layer_clear(&spr0, ppu_getb(0x3f00));
	window_layer_clear(&spr1, ppu_getb(0x3f00));
}

void
ppu_run_cycle()
{
	if (!ppu.ready && cpu_cycles > 29658)
		ppu.ready = 1;

	if (ppu.scanline < 240) {
		/* Rendering! */

		/* If background */
//		if (ppu.PPUMASK & (1 << 3))
//			ppu_draw_bg_line();

		/* If sprite */
		if (ppu.PPUMASK & (1 << 4))
			ppu_draw_sprites_line();
	}
	else if (ppu.scanline == 240) {
		/* Dummy scanline */
	}
	else if (ppu.scanline == 241) {
		/* Start of Vblank! */

		/* In vblank = True */
		ppu.PPUSTATUS |=  (1 << 7);
		/* In Sprite-0 hit = False */
		ppu.PPUSTATUS &= ~(1 << 6);

		/* If should raise nmi */
		if (ppu.PPUCTRL & (1 << 7))
			cpu_nmi();
	}
	else if (ppu.scanline < 261) {
		/* VBLANK! */
	}
	else if (ppu.scanline == 261) {
		/* Dummy scanline */

		printf("start of cadr!\n");
		/* In vblank = False */
		ppu.PPUSTATUS &= ~(1 << 7);
	}

	ppu.scanline++;

	if (ppu.scanline >= PPU_SCANLINE_PF) {
		ppu_draw_screen();
		ppu.scanline = 0;
	}
}

void
ppu_run_cycles(int n)
{
	while (n--)
		ppu_run_cycle();
}

void
ppu_init()
{
	ppu.PPUCTRL = ppu.PPUMASK =
		ppu.PPUSCROLL_X = ppu.PPUSCROLL_Y =
		ppu.PPUADDR = 0;
	ppu.PPUSTATUS = (1 << 7) | (1 << 5);
	ppu.ready = 0;
	ppu.scanline = 0;
}

