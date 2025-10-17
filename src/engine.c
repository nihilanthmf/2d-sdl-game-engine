#include <SDL.h>
#include <stdbool.h>
#include "engine.h"

/// @brief Create a window and a renderer
int init_sdl(SDL_Window **window, SDL_Renderer **renderer, int screen_width, int screen_heght) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // create a window
    *window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_heght, 0);
    if (!*window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    return 0;
}

/// @brief set all the pixels in the screen to 0 (must be called before any rendering every frame)
void clear_screen(int *screen_pixel_buffer, int pixel_count) {
    // clearing the screen
    for (int i = 0; i < pixel_count; ++i) {
        screen_pixel_buffer[i] = 0x000000;
    }
}

/// @brief render the current pixel buffer on the screen
void render_frame(SDL_Renderer *renderer, SDL_Texture *texture, int *pixels, int screen_width) {
    SDL_UpdateTexture(texture, NULL, pixels, screen_width * sizeof(int));

    SDL_RenderCopyEx(
        renderer,
        texture,
        NULL,
        NULL,
        0.0,
        NULL,
        SDL_FLIP_VERTICAL
    );

    SDL_RenderPresent(renderer);
}

/// @brief Loads a sprite out of a supplied .bmp file
/// @param file_path 
/// @return pointer to the newly created SDL_Surface
SDL_Surface* load_sprite(char* file_path) {
    SDL_Surface* sprite = SDL_LoadBMP(file_path);
    if (sprite == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }
    return sprite;
}

/// @brief An easy way to initialize a GameObject structure
/// @param sprite pointer to SDL_Surface, a sprite of the game object
/// @param x x coordinate
/// @param y x coordinate
/// @return returns a copy of a newly created GameObject struct
GameObject create_gameobject(SDL_Surface* sprite, int x, int y) {
    GameObject game_object;
    game_object.sprite = sprite;
    game_object.x = x;
    game_object.y = y;

    return game_object;
}

/// @brief draw a sprite (SDL_Surface) on a screen
/// @param screen_pixel_buffer 
/// @param sprite 
/// @param pos_x 
/// @param pos_y 
void draw_sprite(int *screen_pixel_buffer, SDL_Surface* sprite, int pos_x, int pos_y, int screen_width, int screen_height) {
    int *pixels = (int*)(sprite->pixels);
    int w = sprite->w;
    int h = sprite->h;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int pixel = pixels[(h-y-1)*h+x];
            int screen_pixel_index = (pos_y + y)*screen_width + x + pos_x;
            if (pixel && screen_pixel_index < screen_width * screen_height) {
                screen_pixel_buffer[screen_pixel_index] = pixel;
            }
        }
    }
}

/// @brief Draws gameobject's sprite using draw_sprite()
/// @param screen_pixel_buffer 
/// @param gameobject 
/// @param screen_width 
/// @param screen_height 
void draw_game_object(int *screen_pixel_buffer, GameObject game_object, int screen_width, int screen_height) {
    draw_sprite(screen_pixel_buffer, game_object.sprite, game_object.x, game_object.y, screen_width, screen_height);
}

/// @brief Check whether the supplied key is currently being pressed
/// @param key a string containing human readable key name
/// @param keyboard_state current state of the keyboard from SDL_GetKeyboardState
/// @return 0 if not presses, 1 if presses, -1 if the supplied key does not exist
int get_key(char *key, const Uint8* keyboard_state) {
    int scancode = SDL_GetScancodeFromName(key);
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        printf("Key %s is invalid!", key);
        return -1;
    }
    return keyboard_state[scancode];
}

bool collide(GameObject a, GameObject b) {
    if (b.x + b.sprite->w > a.x && b.x < a.x + a.sprite->w) { // check X coordinate collision
        // if (b.y > a.y && b.y < a.y + a.sprite->h) {
            return true;
        // }
    }
    return false;
}