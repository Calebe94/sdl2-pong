/*************************
 * INCLUDES
 *************************/
#include <SDL2/SDL_rect.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "SDL2/SDL.h"

/*************************
 * MACROS
 *************************/
#define WIDTH         800
#define HEIGHT        600
#define BALL_SIZE     10
#define BALL_SPEED    120
#define PLAYER_WIDTH  20
#define PLAYER_HEIGHT 75
#define PLAYER_MARGIN 10
#define PLAYER_SPEED  150.0f

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

typedef struct player {
    int score;
    float y;
    float x;
} player_t;

/*************************
 * GLOBAL Variables
 *************************/
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
bool run_game = true;
ball_t ball = { 0 };
player_t player1 = { 0 };
player_t player2 = { 0 };

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

/******** Players *********/
player_t create_player(void);

void update_players(float elapsed);

void render_players(void);

void move_player_up(player_t *player);

void move_player_down(player_t *player);

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
        player1 = create_player();
        player2 = create_player();
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
    update_players(elapsed);
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

void game_render()
{
    SDL_SetRenderDrawColor(renderer, BLACK);
    SDL_RenderClear(renderer);

    render_ball(&ball);
    render_players();

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
        switch(event.type)
        {
            case SDL_QUIT: {
                run_game = false;
                SDL_Log("Closing the game...");
                break;
            }
            case SDL_KEYDOWN: {
                switch (event.key.keysym.sym) {
                    case SDLK_w: {
                        SDL_Log("Player 1 UP");
                        move_player_up(&player1);
                        break;
                    }
                    case SDLK_s: {
                        SDL_Log("Player 1 DOWN");
                        move_player_down(&player1);
                        break;
                    }
                    case SDLK_UP: {
                        SDL_Log("Player 2 UP");
                        move_player_up(&player2);
                        break;
                    }
                    case SDLK_DOWN: {
                        SDL_Log("Player 2 DOWN");
                        move_player_down(&player2);
                        break;
                    }
                    break;
                }
            }
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

    if (ball->x < ((float)BALL_SIZE/2))
    {
        ball->x_speed = fabs(ball->x_speed);
    }
    if( ball->x > WIDTH - ((float)BALL_SIZE/2) )
    {
        ball->x_speed = -fabs(ball->x_speed);
    }

    if (ball->y < ((float)BALL_SIZE/2))
    {
        ball->y_speed = fabs(ball->y_speed);
    }
    if( ball->y > HEIGHT - ((float)BALL_SIZE/2) )
    {
        ball->y_speed = -fabs(ball->y_speed);
    }
}

/******** Players *********/
player_t create_player(void)
{
    player_t player = {
        .y = ((float)HEIGHT / 2),
    };

    return player;

}

void update_players(float elapsed)
{
    if (player1.y < ((float)PLAYER_HEIGHT/2))
    {
        player1.y = ((float)PLAYER_HEIGHT/2);
    }
    if (player1.y > (HEIGHT - ((float)PLAYER_HEIGHT/2)) )
    {
        player1.y = (HEIGHT - ((float)PLAYER_HEIGHT/2));
    }
    if (player2.y < ((float)PLAYER_HEIGHT/2))
    {
        player2.y = ((float)PLAYER_HEIGHT/2);
    }
    if (player2.y > (HEIGHT - ((float)PLAYER_HEIGHT/2)) )
    {
        player2.y = (HEIGHT - ((float)PLAYER_HEIGHT/2));
    }
}

void render_players(void)
{
    SDL_SetRenderDrawColor(renderer, BLUE);
    SDL_Rect player1_rect = {
        .x = PLAYER_MARGIN,
        .y = player1.y - ((float)PLAYER_HEIGHT/2),
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &player1_rect);

    SDL_SetRenderDrawColor(renderer, RED);
    SDL_Rect player2_rect = {
        .x = WIDTH - PLAYER_WIDTH - PLAYER_MARGIN,
        .y = player2.y - ((float)PLAYER_HEIGHT/2),
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &player2_rect);
}

void move_player_up(player_t *player)
{
    player->y -= 20;
}

void move_player_down(player_t *player)
{
    player->y += 20;
}
