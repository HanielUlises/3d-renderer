#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "display.h"
#include "vector.h"

bool is_running = false;

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        break;        
    }
}

int main(void) {
    is_running = initialize_window();

    setup();
    while(is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();
    return 0;
}