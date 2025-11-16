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

/// @brief Create a SDL_Surface sprite of one character using a provided font (the font has to be a .bmp file with one line of characters in ASCII order, to generate a nice font file: https://stmn.itch.io/font2bitmap)
/// @param c character to create a sprite out of
/// @param font pointer to SDL_Surface with the font to use
/// @param font_character_grid_size // width and height of a letter image in a font file, including spaces between characters
/// @return pointer to SDL_Surface with character sprite; NULL if supplied a character that is not in the font file
SDL_Surface* create_text_character_sprite(char c, SDL_Surface *font, int font_character_grid_size) {
    int ascii_start = 32; // the first ASCII character included in the font file (32 for Space character), included
    int ascii_end = 126; // the last ASCII character included in the font file (32 for Space character), included

    if (c > ascii_end) {
        return NULL;
    }

    int font_character_index = c - ascii_start;

    SDL_Surface *character_sprite = SDL_CreateRGBSurfaceWithFormat(0, font_character_grid_size, font_character_grid_size, 
                                                                 font->format->BitsPerPixel, font->format->format);
    
    for (int y = 0; y < font_character_grid_size; ++y) {
        for (int x = 0; x < font_character_grid_size; ++x) {
            int font_index = y * font->w + (font_character_index * font_character_grid_size + x);
            ((int*)(character_sprite->pixels))[y * font_character_grid_size + x] = ((int*)(font->pixels))[font_index];
        }
    }

    return character_sprite;
}

/// @brief Create a (!! one line) of text
/// @param text text to create a sprite of
/// @param text_length length of the provided text
/// @return a pointer to SDL_Surface with text sprite
SDL_Surface* create_text_sprite(char *text, int text_length, SDL_Surface *font, int font_character_grid_size) {
    SDL_Surface *text_sprite = SDL_CreateRGBSurfaceWithFormat(0, font_character_grid_size * text_length, font_character_grid_size, 
                                                                 font->format->BitsPerPixel, font->format->format);

    for (int i = 0; i < text_length; ++i) {
        char c = text[i];

        SDL_Surface *character_sprite = create_text_character_sprite(c, font, font_character_grid_size);

        for (int y = 0; y < font_character_grid_size; ++y) {
            for (int x = 0; x < font_character_grid_size; ++x) {
                int text_sprite_index = y * text_sprite->w + (i * font_character_grid_size + x);
                ((int*)(text_sprite->pixels))[text_sprite_index] = 
                    ((int*)(character_sprite->pixels))[y * font_character_grid_size + x];
            }
        }
    }

    return text_sprite;
}
