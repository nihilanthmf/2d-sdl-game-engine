#include <SDL.h>
#include <stdbool.h>
#include "../src/engine.h"
#include "../src/utilities.h"

RenderingComponents rendering_components;

SDL_DisplayMode display_mode;
int screen_width;
int screen_height;

GameObject background;
GameObject monitor;
GameObject lamp;
GameObject text;

SDL_Surface *lamp_off;
SDL_Surface *lamp_on;

bool drag_and_drop_mouse_pressed;
long current_time = 0;

// Function that gets called once after the start of the program, before update()
void start() {
    rendering_components = init_rendering();
    display_mode = get_screen_size();

    screen_width = display_mode.w;
    screen_height = display_mode.h;

    background = create_gameobject(load_sprite("../example/art/background.bmp"), 100, 200);

    SDL_Surface *font = load_sprite("../example/art/font.bmp");
    int font_character_grid_size = 30; // width and height of a letter image in a font file, including spaces between characters
    text = create_gameobject(create_text_sprite("Day 1", 5, font, font_character_grid_size), 100, 100);

    monitor = create_gameobject(resize_sprite(load_sprite("../example/art/monitor.bmp"), 8), 0, 0);

    lamp_off = load_sprite("../example/art/lamp-off.bmp");
    lamp_on = load_sprite("../example/art/lamp-on.bmp");
    lamp = create_gameobject(resize_sprite(lamp_off, 1), 100, 500);

    monitor.x = display_mode.w/2 - monitor.sprite->w/2;
    monitor.y = display_mode.h/2 + monitor.sprite->h/2 - 50;

    drag_and_drop_mouse_pressed = false;
}

// Function that gets called every frame, used for game logic
void update(bool running) {
    long updated_time = get_current_time_in_ms();
    int fps = 1000 / (updated_time - current_time);
    current_time = updated_time;

    if (get_key_down(SDL_SCANCODE_SPACE)) {
        lamp.sprite = lamp.sprite == lamp_on ? lamp_off : lamp_on;
    }

    int x, y;
    int mouse_state = get_mouse(&x, &y);
}

// Function that gets called every frame after update(), used to render to the screen
void render() {
    // draw the background full screen, draw the middle of the image if it's bigger than the screen's resolution
    draw_sprite(rendering_components.pixels, background.sprite, 
        (screen_width-background.sprite->w)/2, screen_height + (background.sprite->h-screen_height)/2,
        screen_width, screen_height);

    draw_game_object(&rendering_components, monitor);
    draw_game_object(&rendering_components, lamp);
    draw_game_object(&rendering_components, text);

    render_frame(rendering_components.renderer, rendering_components.texture, rendering_components.pixels, screen_width);
}

int main(int argc, char **argv) {
    game_loop(start, update, render);

    return 0;
}
