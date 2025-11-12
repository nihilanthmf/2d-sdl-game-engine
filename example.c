#include <SDL.h>
#include <stdbool.h>
#include "src/engine.h"

typedef struct Player {
    GameObject game_object;
    int health;
} Player;

int main(int argc, char **argv) {
    RenderingComponents rendering_components = init_rendering();
    SDL_Event event;
    // Get the keyboard state array (valid for the lifetime of the program)
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    SDL_DisplayMode display_mode = get_screen_size();
    int screen_width = display_mode.w;
    int screen_height = display_mode.h;

    Player player;
    player.game_object = create_gameobject(load_sprite("../art/heart.bmp"), screen_width/2, screen_height/2);
    player.health = 10;

    GameObject enemy = create_gameobject(load_sprite("../art/heart.bmp"), 300, 400);
    GameObject enemy_2 = create_gameobject(load_sprite("../art/heart.bmp"), 100, 200);
    GameObject background = create_gameobject(load_sprite("../art/background.bmp"), 100, 200);

    int number_of_non_movable_go = 1;
    // malloc an array of pointers to gameobjects
    GameObject **non_movable_objects_list = malloc(sizeof(*non_movable_objects_list) * number_of_non_movable_go);
    if (non_movable_objects_list == NULL) {
        printf("Something went wrong!\n");
        exit(1);
    }

    non_movable_objects_list[0] = &enemy_2;

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

        if (get_key("e", keys)) {
            simulate_camera_movement(non_movable_objects_list, number_of_non_movable_go, 2);
        }

        int x, y;
        int mouse_state = get_mouse(&x, &y);
        int mouse_enemy_collision_status = mouse_gameobject_collision(x, y, &enemy);

        if (collide(player.game_object, enemy)) {
            player.health--;
            printf("%d\n", player.health);
        }

        // draw the background full screen, draw the middle of the image if it's bigger than the screen's resolution
        draw_sprite(rendering_components.pixels, background.sprite, 
            (screen_width-background.sprite->w)/2, (screen_height-background.sprite->h)/2,
            screen_width, screen_height);

        draw_game_object(&rendering_components, player.game_object);
        draw_game_object(&rendering_components, enemy);
        draw_game_object(&rendering_components, enemy_2);

        render_frame(rendering_components.renderer, rendering_components.texture, rendering_components.pixels, screen_width);

        SDL_Delay(10);
    }

    return 0;
}
