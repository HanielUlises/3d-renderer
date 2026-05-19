build:
	gcc -Wall -std=c99 -I./include ./src/*.c -o renderer

run:
	./renderer

clean:
	rm renderer