#include <SDL.h>
#include <stdbool.h>
#include <math.h>
#include "../src/engine.h"
#include "../src/utilities.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

int main(int argc, char **argv) {
    // maybe malloc init_rendering result in the engine.c??
    RenderingComponents rendering_components = init_rendering(SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Event event;
    // Get the keyboard state array (valid for the lifetime of the program)
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    int cell_size = 50;
    int map[SCREEN_HEIGHT/cell_size][SCREEN_WIDTH/cell_size];

    // game loop
    bool running = true;
    while (running) {
        // clear_screen(rendering_components.pixels, rendering_components.pixel_count);

        // handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        int mouse_x, mouse_y;
        int mouse_state = get_mouse(&mouse_x, &mouse_y);

        // draw a square strictly in a grid when left mouse button is pressed
        if (mouse_state == 1) {
            int color = get_key("d", keys) == 1 ? 0x000000 : 0xff0000;
            int x = round(((float)mouse_x) / cell_size) * cell_size;
            int y = round(((float)mouse_y) / cell_size) * cell_size;
            if (x-cell_size/2 > 0 && x+cell_size/2 < SCREEN_WIDTH && y-cell_size/2 > 0 && y+cell_size/2 < SCREEN_HEIGHT) {
                draw_rectangle(cell_size, cell_size, x, y, color, SCREEN_WIDTH, SCREEN_HEIGHT, rendering_components.pixels);
                map[y/cell_size][x/cell_size] = 1; 
            }
        }

        render_frame(rendering_components.renderer, rendering_components.texture, rendering_components.pixels, SCREEN_WIDTH);

        SDL_Delay(10);
    }

    return 0;
}
