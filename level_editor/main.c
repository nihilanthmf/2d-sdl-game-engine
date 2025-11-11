#include <SDL.h>
#include <stdbool.h>
#include "../src/engine.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

typedef struct Player {
    GameObject game_object;
    int health;
} Player;

int main(int argc, char **argv) {
    RenderingComponents rendering_components = init_rendering(SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Event event;
    // Get the keyboard state array (valid for the lifetime of the program)
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    // game loop
    bool running = true;
    while (running) {
        clear_screen(rendering_components.pixels, rendering_components.pixel_count);

        // handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        render_frame(rendering_components.renderer, rendering_components.texture, rendering_components.pixels, SCREEN_WIDTH);

        SDL_Delay(10);
    }

    return 0;
}
