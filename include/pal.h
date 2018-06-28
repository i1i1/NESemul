#ifndef _PAL_H_
#define _PAL_H_

#define PAL

/* Frame per second */
#define WINDOW_FRAME_PS		50

#define WINDOW_SURW		(256 * WINDOW_PXL_SZ)
#define WINDOW_SURH		(240 * WINDOW_PXL_SZ)

/* Scanline per frame (of which is V-Blank) */
#define PPU_SCANLINE_PF		312
#define PPU_CYCLES_VBLNK	70

/* CPU cycles per scanline */
#define CPU_CYCLES_PVBLNK	106.56

#define CPU_FREQ		(1.79 * 1000 * 1000)

#endif /* _PAL_H_ */
