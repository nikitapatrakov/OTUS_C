CC = clang
CFLAGS = -std=c99 -Wall -Wextra -DGL_SILENCE_DEPRECATION $(shell pkg-config --cflags sdl2)
LDFLAGS = $(shell pkg-config --libs sdl2) -framework OpenGL

SRC = main.c
OUT = app

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
