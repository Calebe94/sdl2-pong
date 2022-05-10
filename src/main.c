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
#define WIDTH      800
#define HEIGHT     600

/******** Colors *********/
#define BLACK      0, 0, 0, 0
#define BLUE       0, 0, 255, 255
#define RED        255, 0, 0, 255

/*************************
 * GLOBAL Variables
 *************************/
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool run_game = true;

/*************************
 * Typedefs
 *************************/
typedef struct ball {
    float x;
    float y;
    float x_speed;
    float y_speed;
    int size;
} ball_t;

/*************************
 * Prototypes
 *************************/

/******** Game ***********/
bool game_initialize(void);

void game_update(float elapsed);

void game_render(float elapsed);

void game_close(void);

void game_loop(void);

void handle_events(void);

/******** Entities *********/
ball_t create_ball(int size);

void create_object();

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

}

void game_render(float elapsed)
{
    SDL_SetRenderDrawColor(renderer, BLACK);

    SDL_RenderClear(renderer);

    create_object();
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
        game_render(elapsed_time);
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
        .x = (WIDTH/2) / (size/2),
        .y = (HEIGHT/2) / (size/2),
        .size = 10,
    };

    return ball;
}
