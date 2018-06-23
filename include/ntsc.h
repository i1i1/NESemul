#ifndef _NTSC_H_
#define _NTSC_H_

#include "common.h"

/* Frame per second */
#define WINDOW_FRAME_PS		60

#define WINDOW_SURW		(256 * WINDOW_PXL_SZ)
#define WINDOW_SURH		(224 * WINDOW_PXL_SZ)

/* Scanline per frame (of which is V-Blank) */
#define PPU_SCANLINE_PF		262
#define PPU_CYCLES_VBLNK	20

/* CPU cycles per scanline */
#define CPU_CYCLES_PVBLNK	113.33

#define CPU_FREQ		(1.79 * 1000 * 1000)

#endif /* _NTSC_H_ */
