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

	for (i = 0; i < WINDOW_SURH; i++) {
		for (j = 0; j < WINDOW_SURW; j++) {
			c = ppu_palette[lp->arr[i][j]];
			SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 0);

			for (k = 0; k < WINDOW_PXL_SZ; k++)
				for (l = 0; l < WINDOW_PXL_SZ; l++)
					SDL_RenderDrawPoint(renderer,
							    j * WINDOW_PXL_SZ + k,
							    i * WINDOW_PXL_SZ + l);
		}
	}
}

void
window_flush()
{
	SDL_RenderPresent(renderer);
}

