#include <SDL.h>
#include <stdbool.h>
#include "engine.h"

/// @brief Create a window and a renderer
int init_sdl(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // create a window
    *window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    // *window = SDL_CreateWindow("Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920/5*2, 1080/5*2, 0);
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

/// @brief A function that controls the game loop
/// @param start pointer to a function that gets called once before the first frame every gets drawn, before first update() gets called
/// @param update pointer to a function that gets called every frame, after start()
void game_loop(void (*start)(), void (*update)(bool), void (*render)()) {
    start();

    bool running = true;
    SDL_Event event;
    while (running) {
        // handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        update(running);
        render();
        SDL_Delay(10);
    }
}

/// @brief Get the screen size
/// @return SDL_DisplayMode instance with w and h representing width and height of the screen respectively
SDL_DisplayMode get_screen_size() {
    SDL_DisplayMode display_mode;
    if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0) {
        printf("SDL_GetCurrentDisplayMode failed: %s\n", SDL_GetError());
    }
    return display_mode;
}

/// @brief Sets all the pixels in the screen to 0 (must be called before any rendering every frame)
void clear_screen(int *screen_pixel_buffer, int pixel_count) {
    // clearing the screen
    for (int i = 0; i < pixel_count; ++i) {
        screen_pixel_buffer[i] = 0x000000;
    }
}

/// @brief Renders the current pixel buffer on the screen
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

/// @brief Creates a window and initialises basic rendering components
/// @param screen_width
/// @param screen_height
/// @return RenderingComponents struct with necessary rendering components
RenderingComponents init_rendering() {
    RenderingComponents rendering_components;

    // Declaring the basic SDL variables
    SDL_Window *window;
    SDL_Renderer *renderer;

    // initialize window and renderer
    int init_sdl_status = init_sdl(&window, &renderer);
    if (init_sdl_status != 0) {
        exit(1);
    }

    SDL_DisplayMode display_mode = get_screen_size();
    int screen_width = display_mode.w;
    int screen_height = display_mode.h;

    int pixels[screen_height * screen_width];
    memset(pixels, 0x000000, sizeof(pixels));

    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        screen_width, screen_height
    );

    rendering_components.screen_width = screen_width;
    rendering_components.screen_height = screen_height;
    rendering_components.pixels = pixels;
    rendering_components.pixel_count = sizeof(pixels) / sizeof(pixels[0]); // pixels (in bytes) / size of one pixel (in bytes)
    rendering_components.window = window;
    rendering_components.renderer = renderer;
    rendering_components.texture = texture;

    return rendering_components;
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
    SDL_Surface* optimized = SDL_ConvertSurfaceFormat(sprite, SDL_PIXELFORMAT_ARGB8888, 0);
    if (optimized == NULL) {
        printf("%s\n", SDL_GetError());
        exit(1);
    }
    return optimized;
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
/// @param screen_width 
/// @param screen_height 
void draw_sprite(int *screen_pixel_buffer, SDL_Surface* sprite, int pos_x, int pos_y, int screen_width, int screen_height) {
    int *pixels = (int*)(sprite->pixels);
    int w = sprite->w;
    int h = sprite->h;

    pos_y = screen_height - pos_y;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int pixel = pixels[(h-y-1)*w+x];
            int screen_pixel_index = (pos_y + y)*screen_width + x + pos_x;
            if (pixel && screen_pixel_index < screen_width * screen_height &&
                pos_x + x > 0 && pos_x + x < screen_width &&
                pos_y + y > 0 && pos_y + y < screen_height) {
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
void draw_game_object(RenderingComponents *rendering_components, GameObject game_object) {
    draw_sprite(rendering_components->pixels, game_object.sprite, game_object.x, game_object.y, rendering_components->screen_width, rendering_components->screen_height);
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

/// @brief Checks whether the supplised key has been clicked
/// @param key a string containing human readable key name
/// @param keyboard_state current state of the keyboard from SDL_GetKeyboardState
/// @return 0 if not presses, 1 if presses, -1 if the supplied key does not exist
int get_key_down(char *key, const Uint8* keyboard_state) {
    int scancode = SDL_GetScancodeFromName(key);
    if (scancode == SDL_SCANCODE_UNKNOWN) {
        printf("Key %s is invalid!", key);
        return -1;
    }
    bool key_state = keyboard_state[scancode];
    if (key_state == true) {
        // SDL_Event event = {.type = SDL_KEYUP, .key.keysym.scancode = scancode, .key.state = SDL_RELEASED, .key.repeat};
        SDL_Event event;
        event.type = SDL_KEYUP;
        event.key.keysym.sym = scancode;  // the key you want to reset
        event.key.state = SDL_RELEASED;
        event.key.repeat = 0;
        SDL_PushEvent(&event);
    }
    return key_state;
}

/// @brief Get the mouse position & state
/// @param x a pointer to a variable that will hold the "X" axis mouse position
/// @param y a pointer to a variable that will hold the "Y" axis mouse position
/// @return mouse button state
int get_mouse(int *x, int *y) {
    int scancode = SDL_GetMouseState(x, y);
    return scancode;
}

/// @brief Check whether the mouse collides with a GameObject
/// @param x current X coordinate of a mouse
/// @param y current Y coordinate of a mouse
/// @param game_object a pointer to the GameObject
/// @return 1 if collides, 0 otherwise
int mouse_gameobject_collision(int x, int y, GameObject *game_object) {
    if (x >= game_object->x && x <= (game_object->x + game_object->sprite->w) &&
        y <= game_object->y && y >= (game_object->y - game_object->sprite->h)
    ) {
        return true;
    }
    return false;
}

/// @brief Checks whether gameobject A collides with gameobject B based on their sprite size and position
/// @param a 
/// @param b 
/// @return true if they collide, false otherwise
bool collide(GameObject a, GameObject b) {
    if (
        (a.x < b.x+b.sprite->w && a.x+a.sprite->w > b.x) && 
        (a.y < b.y+b.sprite->h && a.y+a.sprite->h > b.y)) {
        return true;
    }
    return false;
}

