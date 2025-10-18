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
    RenderingComponents rendering_components = init_rendering(SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Event event;
    // Get the keyboard state array (valid for the lifetime of the program)
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    Player player;
    player.game_object = create_gameobject(load_sprite("../art/heart.bmp"), SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    player.health = 10;

    GameObject enemy = create_gameobject(load_sprite("../art/heart.bmp"), 300, 200);

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

        if (get_key_down("e", keys)) {
            printf("E pressed\n");
        }

        if (collide(player.game_object, enemy)) {
            player.health--;
            printf("%d\n", player.health);
        }

        draw_game_object(rendering_components.pixels, player.game_object, SCREEN_WIDTH, SCREEN_HEIGHT);
        draw_game_object(rendering_components.pixels, enemy, SCREEN_WIDTH, SCREEN_HEIGHT);

        render_frame(rendering_components.renderer, rendering_components.texture, rendering_components.pixels, SCREEN_WIDTH);

        SDL_Delay(10);
    }

    return 0;
}
