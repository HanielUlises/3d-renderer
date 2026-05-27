#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)


extern uint32_t *color_buffer;

extern int window_width;
extern int window_height;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *color_buffer_texture;

bool initialize_window(void);
void setup(void);
void process_input(void);
void update(void);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void draw_grid(int step);
void draw_pixel(int x, int y, uint32_t color);
void draw_rectangle(int x, int y, int height, int width, uint32_t color);
void render(void);
void destroy_window(void);

#endif // DISPLAY_H