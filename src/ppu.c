#include "common.h"
#include "ram.h"
#include "cpu.h"
#include "ppu.h"

struct ppu ppu;

/* I picked 2C02 palette,
 * cause not much difference
 * between all of them.
 */
const byte ppu_palette[3 * 0x40] = {

	/* 00 to 0f */
	84, 84, 84,
	0, 30, 116,
	8, 16, 144,
	48, 0, 136,
	68, 0, 100,
	92, 0, 48,
	84, 4, 0,
	60, 24, 0,
	32, 42, 0,
	8, 58, 0,
	0, 64, 0,
	0, 60, 0,
	0, 50, 60,
	0, 0, 0,

	/* 10 to 1f */
	152, 150, 152,
	8, 76, 196,
	48, 50, 236,
	92, 30, 228,
	136, 20, 176,
	160, 20, 100,
	152, 34, 32,
	120, 60, 0,
	84, 90, 0,
	40, 114, 0,
	8, 124, 0,
	0, 118, 40,
	0, 102, 120,
	0, 0, 0,

	/* 20 to 2f */
	236, 238, 236,
	76, 154, 236,
	120, 124, 236,
	176, 98, 236,
	228, 84, 236,
	236, 88, 180,
	236, 106, 100,
	212, 136, 32,
	160, 170, 0,
	116, 196, 0,
	76, 208, 32,
	56, 204, 108,
	56, 180, 204,
	60, 60, 60,

	/* 30 to 3f */
	236, 238, 236,
	168, 204, 236,
	188, 188, 236,
	212, 178, 236,
	236, 174, 236,
	236, 174, 212,
	236, 180, 176,
	228, 196, 144,
	204, 210, 120,
	180, 222, 120,
	168, 226, 144,
	152, 226, 180,
	160, 214, 228,
	160, 162, 160,
};


byte
ppu_reg_get(word addr)
{
	byte ret;

	/* If some WO register */
	if (addr >= 0x4000)
		return 0;
	/* Some mapping */
	else
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
	/* If some WO register or invalide register */
	default:
		todo();
		return 0;
	}
}

void
ppu_load_spr_ram(word addr)
{
	int i;

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

	/* Mirroring */
	if (0x3f20 <= a)
		a = 0x3f20 + a % 0x20;

	/* Mirroring */
	if (0x3000 <= a && a < 0x3f00)
		a = 0x2000 + a % 0x1000;

	/* If not a nametable */
	if (!(0x2000 <= a && a < 0x3000))
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
	if (addr >= 0x4000 && (addr != OAMDMA))
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

void
ppu_run_cycle()
{
	if (!ppu.ready && cpu_cycles > 29658)
		ppu.ready = 1;

	ppu.scanline++;

	if (ppu.scanline == PPU_SCANLINE_PF - PPU_CYCLES_VBLNK - 1) {
		/* In vblank = True */
		ppu.PPUSTATUS |=  (1 << 7);
		/* In Sprite-0 hit = False */
		ppu.PPUSTATUS &= ~(1 << 6);

		/* If should raise nmi */
		if (ppu.PPUCTRL & (1 << 7))
			cpu_nmi();
	}
	else if (ppu.scanline == PPU_SCANLINE_PF) {
		ppu.scanline = 0;

		/* In vblank = False */
		ppu.PPUSTATUS &= ~(1 << 7);
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
}

