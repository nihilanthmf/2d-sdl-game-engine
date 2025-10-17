#include <SDL.h>
#include <stdbool.h>
#include "src/engine.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

typedef struct Player {
    GameObject game_object;
    int health;
} Player;

int main(int argc, char **argv) {
    // Declaring the basic SDL variables
    SDL_Window *window;
    SDL_Renderer *renderer;

    int pixels[SCREEN_HEIGHT * SCREEN_WIDTH];
    // pixels (in bytes) / size of one pixel (in bytes)
    const int pixel_count = sizeof(pixels) / sizeof(pixels[0]);
    memset(pixels, 0x000000, sizeof(pixels));

    // initialize window and renderer
    int init_sdl_status = init_sdl(&window, &renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (init_sdl_status != 0) {
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT
    );
    SDL_Event event;

    Player player;
    player.game_object = create_gameobject(load_sprite("../art/gun.bmp"), SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    player.health = 10;

    GameObject enemy = create_gameobject(load_sprite("../art/heart.bmp"), 300, 200);

    // Get the keyboard state array (valid for the lifetime of the program)
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    // game loop
    bool running = true;
    while (running) {
        clear_screen(pixels, pixel_count);

        // handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        if (get_key("d", keys) == 1) {
            player.game_object.x += 5;
        }
        else if (get_key("a", keys) == 1) {
            player.game_object.x -= 5;
        }
        if (get_key("w", keys) == 1) {
            player.game_object.y += 5;
        }
        else if (get_key("s", keys) == 1) {
            player.game_object.y -= 5;
        }

        if (collide(player.game_object, enemy)) {
            player.health--;
            printf("%d\n", player.health);
        }

        draw_game_object(pixels, player.game_object, SCREEN_WIDTH, SCREEN_HEIGHT);
        draw_game_object(pixels, enemy, SCREEN_WIDTH, SCREEN_HEIGHT);

        render_frame(renderer, texture, pixels, SCREEN_WIDTH);

        SDL_Delay(10);
    }

    return 0;
}
