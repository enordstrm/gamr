#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

#define APP_NAME "gamr"
#define PARTICLE_AMOUNT_MAX 20
#define PARTICLE_WIDTH_MAX 25
#define PARTICLE_HEIGHT_MAX 25
#define PARTICLE_SPEED_MAX 25

#define PARTICLE_RGB_R 255
#define PARTICLE_RGB_B 155
#define PARTICLE_RGB_G 25
#define PARTICLE_RGB_A 255

typedef struct {
	int x, y;
} direction_t;


typedef struct {
	float lifetime;
	int speed;
	int x, y;
	int w, h;
	direction_t direction;
} particle_t;

typedef struct {
	int max_size;
	int size;
	void *items;
} vector_t;

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
    SDL_Rect player;
    int up;
    int down;
    int left;
    int right;
    bool quit;
	particle_t particles[PARTICLE_AMOUNT_MAX];
	int current_particle_amount;
} app_t;

vector_t*
new_particle_vector(vector_t* vec, void* type)
{
	vec->max_size = PARTICLE_AMOUNT_MAX;
	vec->items = (particle_t*)malloc(sizeof(type) * vec->max_size);
	return vec;
};

particle_t
spawn_particle(app_t *app, int x, int y, int w, int h)
{
	particle_t particle;

	if (app->current_particle_amount == 20)
	{
		return particle;
	}

	particle.x = x;
	particle.y = y;
	particle.w = w;
	particle.h = h;

	particle.lifetime = 0;
	particle.speed = PARTICLE_SPEED_MAX;
	particle.direction.y = 0;
	particle.direction.x = app->current_particle_amount % 2 == 0 ? 1 : -1;

	app->particles[app->current_particle_amount] = particle;
	app->current_particle_amount += 1;

	if (app->current_particle_amount >= 21)
	{
		app->current_particle_amount = 20;
	}

	return particle;
}
void
render_particle(app_t *app, particle_t particle)
{
    SDL_SetRenderDrawColor(app->renderer, PARTICLE_RGB_R, PARTICLE_RGB_G, PARTICLE_RGB_B, PARTICLE_RGB_A);
	SDL_Rect rect;
	rect.x = particle.x;
	rect.y = particle.y;
	rect.w = particle.w;
	rect.h = particle.h;
    SDL_RenderFillRect(app->renderer, &rect);
}

void
remove_particle(particle_t *particle)
{
	return;
}

void process_particle(particle_t *particle)
{
	particle->x += particle->direction.x * particle->speed;
	particle->y += particle->direction.y * particle->speed;


	if (particle->x - particle->w > SCREEN_WIDTH | particle->x < 0)
	{
		remove_particle(particle);
	}
	else if (particle->y > SCREEN_HEIGHT | particle->y + particle-> h < 0 )
	{
		remove_particle(particle);
	}
}

void
app_render_particles(app_t *app)
{
	if (app->current_particle_amount == 0)
	{
		return;
	}
	for (int i = 0; i < (app->current_particle_amount-1); i++)
	{
		render_particle(app, app->particles[i]);
	}
}

void
app_process_particles(app_t *app)
{
	if (app->current_particle_amount == 0)
	{
		return;
	}
	for (int i = 0; i < (app->current_particle_amount-1); i++)
	{
		process_particle(&app->particles[i]);
	}
}

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
					default:
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
		spawn_particle(app,
				(int)app->player.x + app->player.w/2,
				app->player.y + app->player.h - PARTICLE_HEIGHT_MAX,
				PARTICLE_WIDTH_MAX,
				PARTICLE_HEIGHT_MAX
				);
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
		app_process_particles(app);
		app_render_particles(app);
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
	particle_t particles[20];

    app_init(&app);
    app_run(&app);
    app_fini(&app);

    return 0;
}
