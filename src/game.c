/*************************
 * INCLUDES
 *************************/
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"

#include "config.h"
#include "ball_entity.h"
#include "player_entity.h"
#include "ui.h"
#include "game.h"

/*************************
 * GLOBAL Variables
 *************************/
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool playing = false;
uint32_t rounds = 0;
bool run_game = true;

/*************************
 * Functions definitions
 *************************/
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
                TTF_Init();
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
    if (playing)
    {
        update_ball(&ball, elapsed);
        update_players(elapsed);
        handle_colisions();
    }
}

void game_render()
{
    SDL_SetRenderDrawColor(renderer, BLACK);
    SDL_RenderClear(renderer);

    if (playing == false && rounds == 0)
    {
        render_message("Press space", WIDTH/2 - 300/2, HEIGHT/3 - 40/2, 300, 40);
        render_message("to start", WIDTH/2 - 300/2, HEIGHT/3 + 40/2 , 300, 40);
    }
    else if (playing == false && rounds > 0)
    {
        render_message("Game paused", WIDTH/2 - 300/2, HEIGHT/3 - 40/2, 300, 40);
    }

    render_ball(&ball);
    render_players();
    render_score(player1, WIDTH/2 - (100*2), 0);
    render_score(player2, WIDTH/2 + (100), 0);

    if (should_restart_round)
    {
        char message[10];
        sprintf(message, "%d : %d", player1.score, player2.score);
        render_message(message, WIDTH/2 - 300/2, HEIGHT/3 + 40, 300, 40);
    }
    SDL_RenderPresent(renderer);
}

void game_close(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
}

void game_loop(void)
{
    uint32_t last_tick = SDL_GetTicks();
    while(run_game)
    {
        handle_events();
        uint32_t current_tick = SDL_GetTicks();
        uint32_t elapsed_ticks = current_tick - last_tick;
        /* SDL_Log("%d", elapsed_ticks); */
        float elapsed_time = elapsed_ticks / 1000.0;
        /* SDL_Log("%f", elapsed_time); */
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
        switch(event.type)
        {
            case SDL_QUIT: {
                run_game = false;
                SDL_Log("Closing the game...");
                break;
            }
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE: {
                        playing = !playing;
                        if (rounds == 0)
                        {
                            rounds++;
                        }
                        SDL_Log("Game %s!", playing?"started":"paused");
                        break;
                    }
                    default: break;
                }
            }
            default: break;
        }
    }
}

void restart_round(void)
{
    restart_ball_position(&ball);
    should_restart_round = true;
    pause_timer = SDL_GetTicks();
    rounds++;
    /* restart_player_position(&player1); */
    /* restart_player_position(&player2); */
}

bool coin_flip(void)
{
    return (bool)((rand() % 2) == 1);
}

void handle_colisions(void)
{
    SDL_Rect player1_rect = {
        .x = PLAYER_MARGIN,
        .y = (int)(player1.y - ((float)PLAYER_HEIGHT/2)),
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };

    SDL_Rect ball_rect = {
        .x = ball.x - ((float)ball.size / 2),
        .y = ball.y - ((float)ball.size / 2),
        .w = ball.size,
        .h = ball.size,
    };
    if(SDL_HasIntersection(&ball_rect, &player1_rect))
    {
        ball.x_speed = fabs(ball.x_speed);
    }
    SDL_Rect player2_rect = {
        .x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
        .y = (int)(player2.y - ((float)PLAYER_HEIGHT/2) ),
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };
    if(SDL_HasIntersection(&ball_rect, &player2_rect))
    {
        ball.x_speed = -fabs(ball.x_speed);
    }
}
