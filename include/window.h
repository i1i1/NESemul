#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "common.h"

enum joy_keys {
	JOY_A = 0,
	JOY_B = 1,
	JOY_SELECT = 2,
	JOY_START = 3,
	JOY_UP = 4,
	JOY_DOWN = 5,
	JOY_LEFT = 6,
	JOY_RIGHT = 7,
};

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
extern int joy_keys[];

void window_init();
void window_deinit();

void window_flush();

void window_layer_clear(struct window_layer *lp, byte b);
void window_set_to_layer(struct window_layer *lp);

byte window_event_exit();

byte joy1_read_state();
void joy1_write(byte b);

#endif /* _WINDOW_H_ */
