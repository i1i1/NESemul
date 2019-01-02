#include <SDL2/SDL.h>

#include "ppu.h"
#include "ram.h"
#include "window.h"
#include "common.h"

SDL_Event event;
SDL_Window *window;
SDL_Renderer *renderer;

struct window_layer bg, spr0, spr1;

SDL_GameController *gamepad;

int joy_kbd_keys[] = {
	[JOY_A		] = SDL_SCANCODE_K,
	[JOY_B		] = SDL_SCANCODE_J,
	[JOY_SELECT	] = SDL_SCANCODE_U,
	[JOY_START	] = SDL_SCANCODE_I,
	[JOY_UP		] = SDL_SCANCODE_W,
	[JOY_DOWN	] = SDL_SCANCODE_S,
	[JOY_LEFT	] = SDL_SCANCODE_A,
	[JOY_RIGHT	] = SDL_SCANCODE_D,
};

int joy_gamepad_keys[] = {
	[JOY_A		] = SDL_CONTROLLER_BUTTON_A,
	[JOY_B		] = SDL_CONTROLLER_BUTTON_B,
	[JOY_SELECT	] = SDL_CONTROLLER_BUTTON_GUIDE,
	[JOY_START	] = SDL_CONTROLLER_BUTTON_START,
	[JOY_UP		] = SDL_CONTROLLER_BUTTON_DPAD_UP,
	[JOY_DOWN	] = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
	[JOY_LEFT	] = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
	[JOY_RIGHT	] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
};

void
window_init()
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER);
	SDL_CreateWindowAndRenderer(WINDOW_SURW * WINDOW_PXL_SZ,
									(WINDOW_SURH - 16) * WINDOW_PXL_SZ,
										0, &window, &renderer);

	SDL_RenderPresent(renderer);

	if (SDL_NumJoysticks() >= 1)
		gamepad = SDL_GameControllerOpen(0);
	else
		gamepad = NULL;
}

void
window_deinit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	if (gamepad)
		SDL_GameControllerClose(gamepad);
	SDL_Quit();

	die("Closed window");
}

byte
window_event_exit()
{
	return SDL_PollEvent(&event) && event.type == SDL_QUIT;
}

void
window_set_bg(struct color clr)
{
	SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, 0);
	SDL_RenderClear(renderer);
}

void
window_layer_clear(struct window_layer *lp, byte b)
{
	int i;

	for (i = 0; i < WINDOW_SURH; i++)
		memset(lp->arr[i], b, WINDOW_SURW);
}

void
window_draw_bgtiles()
{
#define ROW(a) a,a,a,a,a,
#define B b,b,b,b,
#define W w,w,w,w,

	int i, j;
	SDL_Rect tmp;
	extern int bg_tiles[30][32];

	struct color b, w;
	b.r = b.g = b.b = 255;
	w.r = w.g = w.b = 0;
	struct color arr[16][40][20] = {
		[0] = {
			ROW({ W W W W W })
			ROW({ W B B B W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B B B W })
			ROW({ W W W W W })
		},
		[1] = {
			ROW({ W W W W W })
			ROW({ W W W B W })
			ROW({ W W B B W })
			ROW({ W B W B W })
			ROW({ W W W B W })
			ROW({ W W W B W })
			ROW({ W W W B W })
			ROW({ W W W W W })
		},
		[2] = {
			ROW({ W W W W W })
			ROW({ W W B W W })
			ROW({ W B W B W })
			ROW({ W W W B W })
			ROW({ W W B W W })
			ROW({ W B W W W })
			ROW({ W W B B W })
			ROW({ W W W W W })
		},
		[3] = {
			ROW({ W W W W W })
			ROW({ W B B W W })
			ROW({ W W W B W })
			ROW({ W W W B W })
			ROW({ W B B B W })
			ROW({ W W W B W })
			ROW({ W B B W W })
			ROW({ W W W W W })
		},
		[4] = {
			ROW({ W W W W W })
			ROW({ W W W B W })
			ROW({ W W B B W })
			ROW({ W B W B W })
			ROW({ W B B B W })
			ROW({ W W W B W })
			ROW({ W W W B W })
			ROW({ W W W W W })
		},
		[5] = {
			ROW({ W W W W W })
			ROW({ W W B B W })
			ROW({ W B W W W })
			ROW({ W B W W W })
			ROW({ W W B W W })
			ROW({ W W W B W })
			ROW({ W B B W W })
			ROW({ W W W W W })
		},
		[6] = {
			ROW({ W W W W W })
			ROW({ W W B B W })
			ROW({ W B W W W })
			ROW({ W B W W W })
			ROW({ W B B W W })
			ROW({ W B W B W })
			ROW({ W W B W W })
			ROW({ W W W W W })
		},
		[7] = {
			ROW({ W W W W W })
			ROW({ W B B B W })
			ROW({ W W W B W })
			ROW({ W W W B W })
			ROW({ W W B W W })
			ROW({ W W B W W })
			ROW({ W B W W W })
			ROW({ W W W W W })
		},
		[8] = {
			ROW({ W W W W W })
			ROW({ W W B W W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B B B W })
			ROW({ W B W B W })
			ROW({ W W B W W })
			ROW({ W W W W W })
		},
		[9] = {
			ROW({ W W W W W })
			ROW({ W W B W W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W W B B W })
			ROW({ W W W B W })
			ROW({ W W B W W })
			ROW({ W W W W W })
		},
		[0xA] = {
			ROW({ W W W W W })
			ROW({ W W B W W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B B B W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W W W W W })
		},
		[0xB] = {
			ROW({ W W W W W })
			ROW({ W B B B W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B B B W })
			ROW({ W B W B W })
			ROW({ W B B B W })
			ROW({ W W W W W })
		},
		[0xC] = {
			ROW({ W W W W W })
			ROW({ W W B B W })
			ROW({ W B W W W })
			ROW({ W B W W W })
			ROW({ W B W W W })
			ROW({ W B W W W })
			ROW({ W W B B W })
			ROW({ W W W W W })
		},
		[0xD] = {
			ROW({ W W W W W })
			ROW({ W B B W W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B W B W })
			ROW({ W B B W W })
			ROW({ W W W W W })
		},
		[0xE] = {
			ROW({ W W W W W })
			ROW({ W B B B W })
			ROW({ W B W W W })
			ROW({ W B W W W })
			ROW({ W B B B W })
			ROW({ W B W W W })
			ROW({ W B B B W })
			ROW({ W W W W W })
		},
		[0xF] = {
			ROW({ W W W W W })
			ROW({ W B B B W })
			ROW({ W B W W W })
			ROW({ W B W W W })
			ROW({ W B B B W })
			ROW({ W B W W W })
			ROW({ W B W W W })
			ROW({ W W W W W })
		},
	};

	tmp.h = tmp.w = 4;

	for (i = 0; i < 30; i++) {
		for (j = 0; j < 32; j++) {
			int a, b;
			int k, l;
			int n;

			n = bg_tiles[i+1][j];

			if (n == 0 || n == 0x5F)
				continue;

			b = n % 16;
			a = n >> 4;

			for (k = 0; k < 40; k++) {
				tmp.y = i * 8 * WINDOW_PXL_SZ + k;
				for (l = 0; l < 20; l++) {
					tmp.x = j * 8 * WINDOW_PXL_SZ + l;
					SDL_SetRenderDrawColor(renderer,arr[a][k][l].r,
													arr[a][k][l].g,
													arr[a][k][l].b,
													0);
					SDL_RenderFillRect(renderer, &tmp);
				}
				for (l = 0; l < 20; l++) {
					tmp.x = j * 8 * WINDOW_PXL_SZ + 20 + l;
					SDL_SetRenderDrawColor(renderer,arr[b][k][l].r,
													arr[b][k][l].g,
													arr[b][k][l].b,
													0);
					SDL_RenderFillRect(renderer, &tmp);
				}
			}
		}
	}
}

void
window_draw_pals()
{

	int i, j, k;
	struct color b, w;
	extern int bg_tiles[30][32];
	SDL_Rect tmp;

	b.r = b.g = b.b = 255;
	w.r = w.g = w.b = 0;

	tmp.h = tmp.w = 8 * WINDOW_PXL_SZ;

	for (i = 0; i < 15; i++) {
		for (j = 0; j < 16; j++) {
			for (k = 0; k < 4; k++) {
				struct color c;
				extern int bg_pals[15][16];

				c = ppu_palette[ppu_getb(bg_pals[i][j] + k)];
				tmp.x = (j * 16 + (k &  1) * 8) * WINDOW_PXL_SZ;
				tmp.y = (i * 16 + (k >> 1) * 8 - 8) * WINDOW_PXL_SZ;

				SDL_SetRenderDrawColor(renderer,c.r, c.g, c.b, 0);
				SDL_RenderFillRect(renderer, &tmp);
			}
		}
	}
}

void
window_draw_borders()
{
	int i;

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);

	for (i = 0; i < WINDOW_SURH - 16; i++) {
		int n;
		extern int p_y;

		n = i + p_y;

		if (n % 8)
			continue;

		SDL_RenderDrawLine(renderer, 0, i * WINDOW_PXL_SZ,
						WINDOW_SURW * WINDOW_PXL_SZ, i * WINDOW_PXL_SZ);
		if (n % 16)
			continue;

		SDL_RenderDrawLine(renderer, 0, i * WINDOW_PXL_SZ + 1,
						WINDOW_SURW * WINDOW_PXL_SZ, i * WINDOW_PXL_SZ + 1);
		SDL_RenderDrawLine(renderer, 0, i * WINDOW_PXL_SZ - 1,
						WINDOW_SURW * WINDOW_PXL_SZ, i * WINDOW_PXL_SZ - 1);
	}

	for (i = 0; i < WINDOW_SURW; i++) {
		int n;
		extern int p_x;

		n = i + p_x;

		if (n % 8)
			continue;

		SDL_RenderDrawLine(renderer,i * WINDOW_PXL_SZ, 0,
									i * WINDOW_PXL_SZ, (WINDOW_SURH - 16) * WINDOW_PXL_SZ);

		if (n % 16)
			continue;

		SDL_RenderDrawLine(renderer,i * WINDOW_PXL_SZ + 1, 0,
									i * WINDOW_PXL_SZ + 1, (WINDOW_SURH - 16) * WINDOW_PXL_SZ);
		SDL_RenderDrawLine(renderer,i * WINDOW_PXL_SZ - 1, 0,
									i * WINDOW_PXL_SZ - 1, (WINDOW_SURH - 16) * WINDOW_PXL_SZ);
	}

}

void
window_set_to_layer(struct window_layer *lp)
{
	int i, j;
	struct color c;

	SDL_Rect tmp;

	tmp.w = tmp.h = WINDOW_PXL_SZ;

	if (!lp)
		return;

	for (i = 0; i < WINDOW_SURH - 16; i++) {
		for (j = 0; j < WINDOW_SURW; j++) {
			if (lp->arr[i][j] == 0xff)
				continue;

			c = ppu_palette[lp->arr[i+8][j]];

			tmp.x = j * WINDOW_PXL_SZ;
			tmp.y = i * WINDOW_PXL_SZ;

			SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 0);
			SDL_RenderFillRect(renderer, &tmp);

		}
	}
//	window_draw_bgtiles();
//	window_draw_pals();
//	window_draw_borders();
}

void
window_flush()
{
	SDL_RenderPresent(renderer);
}

static byte kjoy1;
static byte joy1_pw;

byte
joy1_read_state()
{
	const byte *state;
	int key;

	if (kjoy1 >= 8)
		return 1;

	state = SDL_GetKeyboardState(NULL);;
	key = state[joy_kbd_keys[kjoy1]];

	if (gamepad)
		key |= SDL_GameControllerGetButton(gamepad, joy_gamepad_keys[kjoy1]);

	kjoy1++;

	return key;
}

void
joy1_write(byte b)
{
	b &= 1;
	if (b == 0 && joy1_pw == 1)
		kjoy1 = 0;
	joy1_pw = b;
}

