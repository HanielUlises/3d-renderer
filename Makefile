CC = gcc
CFLAGS = -Wall -std=c99 -I./include `pkg-config --cflags sdl2`
LIBS = `pkg-config --libs sdl2` -lm

build:
	$(CC) $(CFLAGS) ./src/*.c -o renderer $(LIBS)

run:
	./renderer

clean:
	rm -f renderer