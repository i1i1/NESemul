#include <SDL2/SDL.h>

#include "window.h"
#include "common.h"


SDL_Event event;
SDL_Window *window;
SDL_Renderer *renderer;


void
window_init()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_TIMER);
	SDL_CreateWindowAndRenderer(WINDOW_SURW, WINDOW_SURH,
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

