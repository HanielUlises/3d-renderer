#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)

vec3_t cube_points[N_POINTS]; 
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

uint32_t *color_buffer = NULL;

int window_width = 800;
int window_height = 600;

int previous_frame_time;

float fov_factor = 640;

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

// Projected 2D point from a 3D vector
vec2_t project(vec3_t point) {
    vec2_t projected_point = {
        .x = (fov_factor * point.x) / point.z,
        .y = (fov_factor * point.y) / point.z
    };

    return projected_point;
}

void update(void) {
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));

    previous_frame_time = SDL_GetTicks();

    cube_rotation.y += 0.01;
    cube_rotation.z += 0.01;

    for(int i = 0; i < N_POINTS; i++) {
        vec3_t point = cube_points[i];
        vec3_t transformed_point = vec3_rotate_y(point, cube_rotation.y);
        transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

        // Transalte points away from camera
        transformed_point.z -= camera_position.z;

        // Project current point
        vec2_t projected_point = project(transformed_point);
        projected_points[i] = projected_point;
    }
}

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

void draw_grid(int step) {
    clear_color_buffer(0x000000);

    for (int y = 0; y < window_height; y++) {
        for (int x = 0; x < window_width; x++) {
            if(x % step == 0 || y % step == 0)
                color_buffer[(window_width * y) + x] = 0xFFFFFF;
        }
    }
}

void draw_pixel(int x, int y, uint32_t color) {
    if(x >= 0 && x < window_width && y >= 0 && y < window_height) 
        color_buffer[(window_width * y) + x] = color;
}

void draw_rectangle(int x, int y, int height, int width, uint32_t color) {
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            int current_x = x + i;
            int current_y = y + j;
            draw_pixel(current_x, current_y, color);
        }
    }
}

void render(void) {
    // draw_grid(25);

    for(int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i]; 
        draw_rectangle(projected_point.x + (window_width / 2), projected_point.y + (window_height / 2), 4, 4, 0xFFFFFF00);
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

void destroy_window(void) {
    free(color_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}