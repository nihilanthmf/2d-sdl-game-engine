#include <SDL.h>
#include <stdbool.h>

typedef struct GameObject GameObject;

void draw_rectangle(int width, int height, int position_x, int position_y, int color, int screen_width, int screen_height, int *pixels);
void drag_and_drop(GameObject *game_object, bool *mouse_pressed);
long get_current_time_in_ms();
SDL_Surface* create_text_character_sprite(char c, SDL_Surface *font, int font_character_grid_size);
SDL_Surface* create_text_sprite(char *text, int text_length, SDL_Surface *font, int font_character_grid_size);