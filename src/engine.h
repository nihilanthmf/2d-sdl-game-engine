#include <SDL.h>
#include <stdbool.h>

/// @brief Basic rendering components
typedef struct RenderingComponents {
    int screen_width;
    int screen_height;
    int *pixels;
    int pixel_count;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
} RenderingComponents;

typedef struct GameObject {
    SDL_Surface* sprite;
    int x;
    int y;
} GameObject;

int init_sdl(SDL_Window **window, SDL_Renderer **renderer);
void game_loop(void (*start)(), void (*update)(bool), void (*render)());
SDL_DisplayMode get_screen_size();
void clear_screen(int *screen_pixel_buffer, int pixel_count);
void render_frame(SDL_Renderer *renderer, SDL_Texture *texture, int *pixels, int screen_width);
RenderingComponents init_rendering();
SDL_Surface* load_sprite(char* file_path);
GameObject create_gameobject(SDL_Surface* sprite, int x, int y);
void draw_sprite(int *screen_pixel_buffer, SDL_Surface* sprite, int pos_x, int pos_y, int screen_width, int screen_height);
void draw_game_object(RenderingComponents *rendering_components, GameObject game_object);
int get_key(SDL_Scancode key);
int get_key_down(SDL_Scancode key);
int get_mouse(int *x, int *y);
int mouse_gameobject_collision(int x, int y, GameObject *game_object);
bool collide(GameObject a, GameObject b);
SDL_Surface* resize_sprite(SDL_Surface *sprite, float scale);