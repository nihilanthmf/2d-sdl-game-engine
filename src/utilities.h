#include <SDL.h>
#include <stdbool.h>

typedef struct GameObject GameObject;

void draw_rectangle(int width, int height, int position_x, int position_y, int color, int screen_width, int screen_height, int *pixels);
void drag_and_drop(GameObject *game_object, bool *mouse_pressed);