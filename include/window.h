#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "common.h"

/* I emulate NTSC one so the characteristics will be:
 * FPS = 60
 * RES = 256 x 224
 * SPF = 262
 * CPU = 1.79 MHz
 */

#define WINDOW_PXL_SZ	2

#define WINDOW_SURW	(256 * WINDOW_PXL_SZ)
#define WINDOW_SURH	(224 * WINDOW_PXL_SZ)


void window_init();
void window_deinit();

byte window_event_exit();

#endif /* _WINDOW_H_ */
