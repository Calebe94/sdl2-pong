#include <stdio.h>
#include <stdbool.h>
#include "SDL2/SDL.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool run_game = true;

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

void create_object()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect objectRect = {
        .x = (800/2) - (10/2),
        .y = (600/2) - (10/2),
        .w = 10,
        .h = 10,
    };
    SDL_RenderFillRect(renderer, &objectRect);
}

bool game_initialize(void)
{
    bool status = true;
    if(SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        SDL_Log("Initializing game...");
        SDL_Log("Creating window...");
        window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
        if (window)
        {
            SDL_Log("Creating renderer...");
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

void tick(void)
{

}

void render(void)
{

}

void game_close(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void game_loop(void)
{
    while(run_game)
    {
        handle_events();
    }
}

int main(int argc, char *argv[])
{
    atexit(game_close);

    if(argc > 1)
    {
        printf("%s does not handle CLI arguments yet.\n", argv[0]);
    }
    if (game_initialize())
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        SDL_RenderClear(renderer);

        create_object();
        SDL_RenderPresent(renderer);

        game_loop();
    }

    game_close();
    return 0;
}
