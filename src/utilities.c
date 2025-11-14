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
