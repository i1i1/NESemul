#include <SDL2/SDL.h>

#include "window.h"
#include "common.h"


SDL_Event event;
SDL_Window *window;
SDL_Renderer *renderer;

struct window_layer bg, spr0, spr1;

void
window_init()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_TIMER);
	SDL_CreateWindowAndRenderer(WINDOW_SURW * WINDOW_PXL_SZ,
				    WINDOW_SURH * WINDOW_PXL_SZ,
				    0, &window, &renderer);

	SDL_RenderPresent(renderer);
}

void
window_deinit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
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
window_set_to_layer(struct window_layer *lp)
{
	int i, j, k, l;
	struct color c;

	SDL_Rect tmp;

	tmp.w = tmp.h = WINDOW_PXL_SZ;

	if (!lp || !lp->arr)
		return;

	for (i = 0; i < WINDOW_SURH; i++) {
		for (j = 0; j < WINDOW_SURW; j++) {
			if (lp->arr[i][j] == 0xff)
				continue;

			c = ppu_palette[lp->arr[i][j]];

			tmp.x = j * WINDOW_PXL_SZ;
			tmp.y = i * WINDOW_PXL_SZ;

			SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 0);
			SDL_RenderFillRect(renderer, &tmp);
		}
	}
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
	const byte *state = SDL_GetKeyboardState(NULL);

	switch (kjoy1++) {
	case 0: /* A */
		return state[SDL_SCANCODE_K];
	case 1: /* B */
		return state[SDL_SCANCODE_J];
	case 2: /* SELECT */
		return state[SDL_SCANCODE_U];
	case 3: /* START */
		return state[SDL_SCANCODE_I];
	case 4: /* UP */
		return state[SDL_SCANCODE_W];
	case 5: /* DOWN */
		return state[SDL_SCANCODE_S];
	case 6: /* LEFT */
		return state[SDL_SCANCODE_A];
	case 7: /* RIGHT */
		return state[SDL_SCANCODE_D];
	default:
		return 1;
	}
}

void
joy1_write(byte b)
{
	b &= 1;
	if (b == 0 && joy1_pw == 1)
		kjoy1 = 0;
	joy1_pw = b;
}

