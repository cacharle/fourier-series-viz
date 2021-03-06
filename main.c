#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define FRAME_DELAY     5
#define WINDOW_WIDTH    900
#define WINDOW_HEIGHT   600
#define CIRCLE_RAD      100
#define CIRCLE_DIAMETER (CIRCLE_RAD * 2)
#define HISTORY_SIZE    (WINDOW_WIDTH - CIRCLE_DIAMETER)
#define POINT_RAD       3
#define PADDING_SIZE    100
#define SEP_SIZE        (PADDING_SIZE + 100)

#define HISTORY_COLOR 0xffffffff
#define POINT_COLOR 0xffffffff
#define CIRCLE_COLOR 0xffffffff

int    g_history[HISTORY_SIZE] = {0};
double g_angle = 0.0;
int    g_circle_count = 1;

bool   g_pause = false;
bool   g_running = true;

void reset_history(void)
{
	for (size_t i = 0; i < HISTORY_SIZE; i++)
		g_history[i] = WINDOW_HEIGHT;
}

int main(void)
{
	reset_history();

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

	while (g_running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					g_running = false;
					break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_q:
						case SDLK_ESCAPE:
							g_running = false;
							break;
						case SDLK_p:
						case SDLK_SPACE:
							g_pause = !g_pause;
							break;
						case SDLK_j:
						case SDLK_MINUS:
							g_circle_count--;
							if (g_circle_count < 1)
								g_circle_count = 1;
							reset_history();
							break;
						case SDLK_k:
						case SDLK_EQUALS:
							g_circle_count++;
							reset_history();
							break;
					}
			}
		}
		if (g_pause)
		{
			SDL_Delay(FRAME_DELAY);
			continue;
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		/*
		** n = range(1, inf, 2)
		**
		** 4 * sin(n * theta)
		** -----------------
		**     pi
		**
		*/

		double sum_x = 0.0;
		double sum_y = 0.0;
		for (int circle_num = 0; circle_num < g_circle_count; circle_num++)
		{
			double factor = 1 + circle_num * 2;
			double radius = (CIRCLE_RAD - 25) * 4 / (factor * M_PI);
			double x = radius * cos(factor * g_angle);
			double y = radius * sin(factor * g_angle);

			int center_x = CIRCLE_RAD        + sum_x + PADDING_SIZE;
			int center_y = WINDOW_HEIGHT / 2 + sum_y;
			aacircleColor(renderer, center_x, center_y, radius, CIRCLE_COLOR);
			filledCircleColor(renderer, center_x + x, center_y + y, POINT_RAD, POINT_COLOR);
			sum_x += x;
			sum_y += y;
		}
		g_angle += 0.01;

		int draw_point_x = CIRCLE_DIAMETER + SEP_SIZE;
		int draw_point_y = WINDOW_HEIGHT / 2 + sum_y;

		hlineColor(renderer, CIRCLE_RAD + sum_x + PADDING_SIZE, draw_point_x, draw_point_y, HISTORY_COLOR);
		filledCircleColor(renderer, draw_point_x, draw_point_y, POINT_RAD, POINT_COLOR);

		memmove(g_history + 1, g_history, sizeof(g_history[0]) * (HISTORY_SIZE - 1));
		g_history[0] = sum_y;

		for (size_t i = 0; i < HISTORY_SIZE - 1; i++)
		{
			size_t next = i + 1;
			if (g_history[next] == WINDOW_HEIGHT)
				break;
			aalineColor(
				renderer,
				CIRCLE_DIAMETER + i + SEP_SIZE,
				WINDOW_HEIGHT / 2 + g_history[i],
				CIRCLE_DIAMETER + next + SEP_SIZE,
				WINDOW_HEIGHT / 2 + g_history[next],
				HISTORY_COLOR
			);
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(FRAME_DELAY);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
