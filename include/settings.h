#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define WINDOW_PXL_SZ	2

/* Frame per second */
#define WINDOW_FRAME_PS		60

#define WINDOW_SURW		256
#define WINDOW_SURH		240
//#define WINDOW_SURH		224

/* Scanline per frame (of which is V-Blank) */
#define PPU_SCANLINE_PF		262
#define PPU_CYCLES_VBLNK	20

/* CPU cycles per scanline */
#define CPU_CYCLES_P_SCANLINE	113.33

#define CPU_FREQ		(1.79 * 1000 * 1000)


#endif /* _SETTINGS_H_ */
