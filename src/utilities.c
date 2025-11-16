#include <SDL.h>
#include <stdbool.h>
#include "utilities.h"
#include "engine.h"

/// @brief Draws a rectangle to the screen
/// @param width 
/// @param height 
/// @param position_x 
/// @param position_y 
/// @param color 
/// @param screen_width 
/// @param screen_height 
/// @param pixels 
void draw_rectangle(int width, int height, int position_x, int position_y, int color, int screen_width, int screen_height, int *pixels) {
    for (int x = -width/2; x < width/2; ++x) {
        for (int y = -height/2; y < height/2; ++y) {
            int index = screen_width*(screen_height-position_y+y) + position_x + x;
            if (index >= 0 && index < screen_width*screen_height) {
                pixels[index] = color;
            }
        }
    }
}

/// @brief A function to drag and drop a game object
/// @param game_object pointer to the gameobject
/// @param mouse_pressed pointer to a bool variable that hold whether the mouse is being pressed on the gameobject or not, it's needed to keep "holding" the gameobject even if the cursor has already left the sprite's boundaries, should not be defined in the game loop
void drag_and_drop(GameObject *game_object, bool *mouse_pressed) {
    int x, y;
    int mouse_state = get_mouse(&x, &y);

    if (mouse_state == 1) {
        if (mouse_gameobject_collision(x, y, game_object) || *mouse_pressed) {
            game_object->x = x - (game_object->sprite)->w / 2;
            game_object->y = y + (game_object->sprite)->h / 2;
            *mouse_pressed = true;
        }
    } else {
        *mouse_pressed = false;
    }
}

/// @brief Get the current UTC time
/// @return current UTC time in ms
long get_current_time_in_ms() {
    struct timespec ts;

    timespec_get(&ts, TIME_UTC);
    long ms = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
    return ms;
}

/// @brief Create a SDL_Surface sprite of one character using project's font (to generate a nice font file: https://stmn.itch.io/font2bitmap)
/// @param c character to create a sprite out of
/// @return pointer to SDL_Surface with character sprite
SDL_Surface* create_text_character_sprite(char c) {
    SDL_Surface *font = load_sprite("../example/art/font.bmp");
    int grid_size = 30;
    int characters_per_row = 19;

    int ascii_start = 32; // included
    int ascii_end = 126; // included
    if (c > ascii_end) {
        return NULL;
    }

    int font_character_index = c - ascii_start;

    SDL_Surface *character_sprite = SDL_CreateRGBSurfaceWithFormat(0, grid_size, grid_size, 
                                                                 font->format->BitsPerPixel, font->format->format);
    
    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size; ++x) {
            int font_row_index = font_character_index / characters_per_row;
            int font_column_index = font_character_index % characters_per_row;

            int font_index = (font_row_index * grid_size + y) * font->w + (font_column_index * grid_size + x);
            ((int*)(character_sprite->pixels))[y * grid_size + x] = ((int*)(font->pixels))[font_index];
        }
    }

    return character_sprite;
}

