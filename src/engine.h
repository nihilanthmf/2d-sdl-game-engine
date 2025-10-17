#include <SDL.h>
#include <stdbool.h>

typedef struct GameObject {
    SDL_Surface* sprite;
    int x;
    int y;
} GameObject;

int init_sdl(SDL_Window **window, SDL_Renderer **renderer, int screen_width, int screen_heght);
void clear_screen(int *screen_pixel_buffer, int pixel_count);
void render_frame(SDL_Renderer *renderer, SDL_Texture *texture, int *pixels, int screen_width);
SDL_Surface* load_sprite(char* file_path);
GameObject create_gameobject(SDL_Surface* sprite, int x, int y);
void draw_sprite(int *screen_pixel_buffer, SDL_Surface* sprite, int pos_x, int pos_y, int screen_width, int screen_height);
void draw_game_object(int *screen_pixel_buffer, GameObject game_object, int screen_width, int screen_height);
int get_key(char *key, const Uint8* keyboard_state);
bool collide(GameObject a, GameObject b);