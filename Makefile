CC = gcc
CFLAGS = -Wall -Wextra $(shell pkgconf --cflags sdl2 SDL2_gfx)
LDFLAGS = -lm $(shell pkgconf --libs sdl2 SDL2_gfx)

NAME = fourier-series-viz

all: $(NAME)

$(NAME): main.o
	$(CC) -o $@ $(LDFLAGS) $^
