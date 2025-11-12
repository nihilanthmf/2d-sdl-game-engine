#include <SDL.h>
#include <stdbool.h>
#include "utilities.h"

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

