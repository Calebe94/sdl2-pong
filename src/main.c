/*************************
 * INCLUDES
 *************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "SDL2/SDL.h"

/*************************
 * MACROS
 *************************/
#define BALL_SIZE  10
#define BALL_SPEED 120
#define WIDTH      800
#define HEIGHT     600

/******** Colors *********/
#define BLACK      0, 0, 0, 0
#define WHITE      255, 255, 255, 255
#define BLUE       0, 0, 255, 255
#define RED        255, 0, 0, 255

/*************************
 * Typedefs
 *************************/
typedef struct ball_entity {
    float x;
    float y;
    float x_speed;
    float y_speed;
    int size;
} ball_t;

/*************************
 * GLOBAL Variables
 *************************/
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool run_game = true;
ball_t ball = { 0 };

/*************************
 * Prototypes
 *************************/

/******** Game ***********/
bool game_initialize(void);

void game_update(float elapsed);

void game_render();

void game_close(void);

void game_loop(void);

void handle_events(void);

/******** Entities *********/
void create_object();

ball_t create_ball(int size);

void render_ball(const ball_t *ball);

void update_ball(ball_t *ball, float elapsed);

/*************************
 * Main
 *************************/
int main(int argc, char *argv[])
{
    srand((unsigned int)time(NULL));
    atexit(game_close);

    if(argc > 1)
    {
        printf("%s does not handle CLI arguments yet.\n", argv[0]);
    }
    if (game_initialize())
    {
        ball = create_ball(BALL_SIZE);
        game_loop();
    }

    game_close();
    return 0;
}

/*************************************
 * Functions Definitions
 ************************************/

/******** Game ***********/

bool game_initialize(void)
{
    bool status = true;
    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Log("Initializing game...");
        SDL_Log("Creating window...");
        window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
        if (window)
        {
            SDL_Log("Creating renderer...");
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
            if (renderer == NULL)
            {
                SDL_Log("Failed to create renderer!");
                status = false;
            }
            else
            {
                SDL_Log("Initialization done!");
            }
        }
        else
        {
            SDL_Log("Failed to create window!");
            status = false;
        }
    }
    else
    {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        status = false;
    }

    return status;
}

void game_update(float elapsed)
{
    update_ball(&ball, elapsed);
}

void game_render()
{
    SDL_SetRenderDrawColor(renderer, BLACK);
    SDL_RenderClear(renderer);

    render_ball(&ball);

    SDL_RenderPresent(renderer);
}

void game_close(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void game_loop(void)
{
    uint32_t last_tick = SDL_GetTicks();
    while(run_game)
    {
        handle_events();
        uint32_t current_tick = SDL_GetTicks();
        uint32_t elapsed_ticks = current_tick - last_tick;
        SDL_Log("%d", elapsed_ticks);
        float elapsed_time = elapsed_ticks / 1000.0;
        SDL_Log("%f", elapsed_time);
        last_tick = current_tick;
        game_update(elapsed_time);
        game_render();
    }
}

void handle_events(void)
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            run_game = false;
            SDL_Log("Closing the game...");
        }
    }
}

bool coin_flip(void)
{
    return (bool)((rand() % 2) == 1);
}

/******** Entities *********/

void create_object()
{
    SDL_SetRenderDrawColor(renderer, RED);
    SDL_Rect objectRect = {
        .x = (800/2) - (10/2),
        .y = (600/2) - (10/2),
        .w = 10,
        .h = 10,
    };
    SDL_RenderFillRect(renderer, &objectRect);
}

ball_t create_ball(int size)
{
    ball_t ball = {
        .x = (WIDTH/2) - (size/2),
        .y = (HEIGHT/2) - (size/2),
        .size = size,
        .x_speed = BALL_SPEED * (coin_flip()?1:-1),
        .y_speed = BALL_SPEED * (coin_flip()?1:-1),
    };

    return ball;
}

void render_ball(const ball_t *ball)
{
    int size = ball->size;
    SDL_Rect rect = {
        .x = ball->x - (size/2),
        .y = ball->y - (size/2),
        .w = size,
        .h = size
    };
    SDL_SetRenderDrawColor(renderer, WHITE);
    SDL_RenderFillRect(renderer, &rect);
}

void update_ball(ball_t *ball, float elapsed)
{
    ball->x += ball->x_speed * elapsed;
    ball->y += ball->y_speed * elapsed;

    if (ball->x < (BALL_SIZE/2))
    {
        ball->x_speed = fabs(ball->x_speed);
    }
    if( ball->x > (WIDTH - BALL_SIZE)/2 )
    {
        ball->x_speed = -fabs(ball->x_speed);
    }

    if (ball->y < (BALL_SIZE/2))
    {
        ball->y_speed = fabs(ball->y_speed);
    }
    if( ball->y > (HEIGHT - BALL_SIZE)/2 )
    {
        ball->y_speed = -fabs(ball->y_speed);
    }
}
