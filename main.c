#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WINDOW_WIDTH  648
#define WINDOW_HEIGHT 480
#define CIRCLE_SIZE   100
#define HISTORY_SIZE  WINDOW_WIDTH - CIRCLE_SIZE

int g_history[HISTORY_SIZE] = {0};

int main(void)
{
	for (size_t i = 0; i < HISTORY_SIZE; i++)
		g_history[i] = 100;

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		exit(1);
	SDL_Window *window = SDL_CreateWindow(
		"fourier-series-viz",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		0
	);
	if (window == NULL)
		exit(1);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
		exit(1);

	bool running = true;
	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_q:
						case SDLK_ESCAPE:
							running = false;
							break;
					}
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		for (size_t i = 0; i < HISTORY_SIZE; i++)
		{
			SDL_RenderDrawPoint(renderer, CIRCLE_SIZE + i, g_history[i]);
		}


		SDL_RenderPresent(renderer);
		SDL_Delay(5);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
