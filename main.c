#include <SDL2\SDL.h>
#include <stdio.h>
#include <string.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

#define APP_NAME "gamr"

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Rect player;
    int up;
    int down;
    int left;
    int right;
    bool quit;
} app_t;

bool 
app_init(app_t* app) 
{
    const int renderer_flags = SDL_RENDERER_ACCELERATED;
    const int window_flags = 0;

    memset(app, 0, sizeof(*app));

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        goto fail;
    }

    //Create window
    app->window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (app->window == NULL) {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        goto fail;
    } 

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app->renderer = SDL_CreateRenderer(app->window, -1, renderer_flags);

    if (!app->renderer) {
        printf("failed to create renderer: %s\n", SDL_GetError());
        goto fail;
    }

    app->player.x = 0;
    app->player.y = 0;
    app->player.w = 100;
    app->player.h = 100;

    // TODO    IMG_Init(IMG_INIT_PNG);

fail:
    // TODO destroy things
    return false;
}

void
app_fini(app_t* app)
{
    SDL_DestroyWindow(app->window);

    // TODO: Destroy renderer

    //Quit SDL subsystems
    SDL_Quit();
}

static void 
app_prepare_scene(app_t* app) 
{
    SDL_SetRenderDrawColor(app->renderer, 96, 128, 255, 255);
    SDL_RenderClear(app->renderer);
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(app->renderer, &app->player);
}

static void
app_present_scene(app_t* app)
{
    SDL_RenderPresent(app->renderer);
}

static void
app_process_input(app_t* app)
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) { 
        switch (e.type) {
            case SDL_QUIT:
                app->quit = true;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (e.key.repeat) {
                    break;
                }
                switch (e.key.keysym.scancode) {
                    case SDL_SCANCODE_W: 
                        app->up = e.type == SDL_KEYDOWN;
                        break;
                    case SDL_SCANCODE_A: 
                        app->left = e.type == SDL_KEYDOWN;;
                        break;
                    case SDL_SCANCODE_S: 
                        app->down = e.type == SDL_KEYDOWN;
                        break;
                    case SDL_SCANCODE_D: 
                        app->right = e.type == SDL_KEYDOWN;
                        break;
                    case SDL_SCANCODE_Q:
                        app->quit = true;
                        break;
                }

            default:
                break;
        }
    } 

    if (app->up) {
        app->player.y -= app->up;
        app->up += 1;
        if (app->up > 100) {
            app->up = 100;
        }
    }
    if (app->down) {
        app->player.y += app->down;
        app->down += 1;
        if (app->down > 100) {
            app->down = 100;
        }
    }
    if (app->left) {
        app->player.x -= app->left;
        app->left += 1;
        if (app->left > 100) {
            app->left = 100;
        }
    }
    if (app->right) {
        app->player.x += app->right;
        app->right += 1;
        if (app->right > 100) {
            app->right = 100;
        }
    }

    if (app->player.y <= 0) {
        app->player.y = 0;
    }
    if (app->player.y >= SCREEN_HEIGHT - app->player.h) {
        app->player.y = SCREEN_HEIGHT - app->player.h;
    }
    if (app->player.x <= 0) {
        app->player.x = 0;
    }
    if (app->player.x >= SCREEN_WIDTH - app->player.w) {
        app->player.x = SCREEN_WIDTH - app->player.w;
    }
}

void 
app_run(app_t* app) 
{
    for (;;) {
        app_prepare_scene(app);
        app_process_input(app);
        app_present_scene(app);

        if (app->quit) {
            return;
        }

        SDL_Delay(16);
    }
}

int 
main(int argc, char* args[])
{
    app_t app;

    app_init(&app);
    app_run(&app);
    app_fini(&app);

    return 0;
}
