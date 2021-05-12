CC = gcc
CFLAGS = -Wall -Wextra $(shell pkgconf --cflags sdl2)
LDFLAGS = $(shell pkgconf --libs sdl2)

NAME = fourier-series-viz

all: $(NAME)

$(NAME): main.o
	$(CC) -o $@ $(LDFLAGS) $^
