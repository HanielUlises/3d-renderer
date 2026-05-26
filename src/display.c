#include "display.h"
#include "vector.h"

const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS]; 

uint32_t *color_buffer = NULL;

int window_width = 800;
int window_height = 600;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;

bool initialize_window(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }
    window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_BORDERLESS);
    if(!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}


void setup(void) {
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    int point_count = 0;

    for(float x = -1; x <= 1; x += 0.25) {
        for(float y = -1; y <= 1; y += 0.25) {
            for(float z = -1; z <= 1; z += 0.25) {
                vec3_t new_points = {.x = x, .y = y, .z = z};
                cube_points[point_count++] = new_points;
            }  
        }  
    }  
}

void update(void) {}

void render_color_buffer(void) {
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int) (window_width * sizeof(uint32_t)));
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
    for(size_t y = 0; y < window_height; y++) {
        for(size_t x = 0; x < window_width; x++) {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}

void draw_grid(void) {
    clear_color_buffer(0x000000);

    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            if(x % 50 == 0 || y % 50 == 0)
                color_buffer[(window_width * y) + x] = 0xFFFFFF;
        }
    }
}

void draw_pixel(int x, int y, uint32_t color) {
    if(x < window_width && y < window_height) 
        color_buffer[(window_width * y) + x] = color;
}

void draw_rectangle(int x, int y, int height, int width, uint32_t color) {
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            color_buffer[(window_width * current_y) + current_x] = color;
        }
    }
}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 100, 110, 255);
    SDL_RenderClear(renderer);

    draw_grid();
    draw_rectangle(300, 200, 150, 300, 0xFFFF00FF);

    render_color_buffer();
    // clear_color_buffer(0xFFFFFF00);

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}