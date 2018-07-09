#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "common.h"

struct window_layer {
	byte arr[WINDOW_SURH][WINDOW_SURW];
};

extern struct window_layer bg, spr0, spr1;

struct color {
	byte r;
	byte g;
	byte b;
};

extern const struct color ppu_palette[0x40];

void window_init();
void window_deinit();

void window_flush();

void window_layer_clear(struct window_layer *lp, byte b);
void window_set_to_layer(struct window_layer *lp);

byte window_event_exit();

byte joy1_read_state();
void joy1_write(byte b);

#endif /* _WINDOW_H_ */
