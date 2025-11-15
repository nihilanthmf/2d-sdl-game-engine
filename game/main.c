#include <SDL.h>
#include <stdbool.h>
#include "../src/engine.h"
#include "../src/utilities.h"

typedef struct Player {
    GameObject game_object;
    int health;
} Player;

RenderingComponents rendering_components;
// Get the keyboard state array (valid for the lifetime of the program)
const Uint8 *keys;

SDL_DisplayMode display_mode;
int screen_width;
int screen_height;

Player player;

GameObject enemy;
GameObject enemy_2;
GameObject background;

bool drag_and_drop_mouse_pressed;

// Function that gets called once after the start of the program, before update()
void start() {
    rendering_components = init_rendering();
    keys = SDL_GetKeyboardState(NULL);
    display_mode = get_screen_size();

    screen_width = display_mode.w;
    screen_height = display_mode.h;

    player.game_object = create_gameobject(load_sprite("../art/heart.bmp"), screen_width/2, screen_height/2);
    player.health = 10;

    enemy = create_gameobject(load_sprite("../art/heart.bmp"), 300, 400);
    enemy_2 = create_gameobject(load_sprite("../art/heart.bmp"), 100, 200);
    background = create_gameobject(load_sprite("../art/background.bmp"), 100, 200);

    drag_and_drop_mouse_pressed = false;
}

// Function that gets called every frame, used for game logic
void update(bool running) {    
    if (get_key("d", keys) == 1) {
        player.game_object.x += 5;
    }
    else if (get_key("a", keys) == 1) {
        player.game_object.x -= 5;
    }
    if (get_key("w", keys) == 1) {
        player.game_object.y -= 5;
    }
    else if (get_key("s", keys) == 1) {
        player.game_object.y += 5;
    }

    int x, y;
    int mouse_state = get_mouse(&x, &y);
    int mouse_enemy_collision_status = mouse_gameobject_collision(x, y, &enemy);

    drag_and_drop(&enemy, &drag_and_drop_mouse_pressed);

    if (collide(player.game_object, enemy)) {
        player.health--;
        printf("%d\n", player.health);
    }
}

// Function that gets called every frame after update(), used to render to the screen
void render() {
    // draw the background full screen, draw the middle of the image if it's bigger than the screen's resolution
    draw_sprite(rendering_components.pixels, background.sprite, 
        (screen_width-background.sprite->w)/2, screen_height + (background.sprite->h-screen_height)/2,
        screen_width, screen_height);

    draw_game_object(&rendering_components, player.game_object);
    draw_game_object(&rendering_components, enemy);
    draw_game_object(&rendering_components, enemy_2);

    render_frame(rendering_components.renderer, rendering_components.texture, rendering_components.pixels, screen_width);
}

int main(int argc, char **argv) {
    game_loop(start, update, render);

    return 0;
}
