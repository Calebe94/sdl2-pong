/*************************
 * INCLUDES
 *************************/
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

/*************************
 * MACROS
 *************************/
#define WIDTH         800
#define HEIGHT        600
#define BALL_SIZE     10
#define BALL_SPEED    240
#define PLAYER_WIDTH  20
#define PLAYER_HEIGHT 75
#define PLAYER_MARGIN 10
#define PLAYER_SPEED  400.0f

// https://www.wfonts.com/font/8bit-wonder
/* #define FONT_NAME "8-BIT_WONDER.ttf" */
// https://www.fontspace.com/pixeboy-font-f43730
#define FONT_NAME "Pixeboy.ttf"

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
bool playing = false;
bool should_restart_round = false;
uint32_t pause_timer = 0;
uint32_t rounds = 0;

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

void restart_round(void);

/******** Entities *********/
void create_object();

ball_t create_ball(int size);

void render_ball(const ball_t *ball);

void restart_ball_position(ball_t *ball);

void update_ball(ball_t *ball, float elapsed);

void handle_colisions(void);

/******** Players *********/
player_t create_player(void);

void update_players(float elapsed);

void handle_players_bounds(void);

void handle_players_inputs(float elapsed);

void render_players(void);

void move_player_up(player_t *player, float ticks);

void move_player_down(player_t *player, float ticks);

void restart_player_position(player_t *player);

void render_score(player_t player, uint32_t x, uint32_t y);

void render_message(const char *message, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
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
        .x = ((float)WIDTH/2) - ((float)size/2),
        .y = ((float)HEIGHT/2) - ((float)size/2),
        .size = size,
        /* .x_speed = BALL_SPEED * (coin_flip()?1:-1), */
        /* .y_speed = BALL_SPEED * (coin_flip()?1:-1), */
    };

    return ball;
}

void render_ball(const ball_t *ball)
{
    int size = ball->size;
    SDL_Rect rect = {
        .x = ball->x - ((float)size/2),
        .y = ball->y - ((float)size/2),
        .w = size,
        .h = size
    };
    SDL_SetRenderDrawColor(renderer, WHITE);
    SDL_RenderFillRect(renderer, &rect);
}

void restart_ball_position(ball_t *ball)
{
    ball->x = ((float)WIDTH/2) - ((float)ball->size/2);
    ball->y = ((float)HEIGHT/2) - ((float)ball->size/2);
    ball->x_speed = 0;
    ball->y_speed = 0;
}

void update_ball(ball_t *ball, float elapsed)
{
    if (!should_restart_round)
    {
        if (ball->x_speed == 0)
        {
            ball->x_speed = BALL_SPEED * (coin_flip()?1:-1);
        }
        if (ball->y_speed == 0)
        {
            ball->y_speed = BALL_SPEED * (coin_flip()?1:-1);
        }
        ball->x += ball->x_speed * elapsed;
        ball->y += ball->y_speed * elapsed;

        if (ball->x < ((float)BALL_SIZE/2))
        {
            ball->x_speed = fabs(ball->x_speed);
            player2.score++;
            SDL_Log("Score player 2: %d", player2.score);
            restart_round();
        }
        if( ball->x > WIDTH - ((float)BALL_SIZE/2) )
        {
            ball->x_speed = -fabs(ball->x_speed);
            player1.score++;
            SDL_Log("Score player 1: %d", player1.score);
            restart_round();
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
    else
    {
        if (SDL_GetTicks() - pause_timer > 2000)
        {
            should_restart_round = false;
        }

    }
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
    handle_players_inputs(elapsed);
    handle_players_bounds();
}

void handle_players_bounds(void)
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

void handle_players_inputs(float elapsed)
{
    const uint8_t *keyboard_state = SDL_GetKeyboardState(NULL);
    if (keyboard_state[SDL_SCANCODE_W])
    {
        move_player_up(&player1, elapsed);
    }
    if (keyboard_state[SDL_SCANCODE_S])
    {
        move_player_down(&player1, elapsed);
    }
    if (keyboard_state[SDL_SCANCODE_UP])
    {
        move_player_up(&player2, elapsed);
    }
    if (keyboard_state[SDL_SCANCODE_DOWN])
    {
        move_player_down(&player2, elapsed);
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

void move_player_up(player_t *player, float ticks)
{
    player->y -= PLAYER_SPEED * ticks;
}

void move_player_down(player_t *player, float ticks)
{
    player->y += PLAYER_SPEED * ticks;
}

void restart_player_position(player_t *player)
{
    player->y = ((float)HEIGHT / 2);
}

void render_score(player_t player, uint32_t x, uint32_t y)
{
    SDL_Color White = {255, 255, 255, 255};
    char score[128];
    sprintf(score, "%d", player.score);

    TTF_Font *Sans = TTF_OpenFont(FONT_NAME, 24);

    if(Sans)
    {
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, score,  White);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_Rect Message_rect; //create a rect
        Message_rect.x = x;  //controls the rect's x coordinate
        Message_rect.y = y; // controls the rect's y coordinte
        Message_rect.w = 100; // controls the width of the rect
        Message_rect.h = 100; // controls the height of the rect

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        /* SDL_RenderPresent(renderer); */

        // Don't forget to free your surface and texture
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
        TTF_CloseFont(Sans);
    }
}

void render_message(const char *message, uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    SDL_Color White = {255, 255, 255, 255};

    TTF_Font *Sans = TTF_OpenFont(FONT_NAME, 24);

    if(Sans)
    {
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, message,  White);
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
        SDL_Rect Message_rect; //create a rect
        Message_rect.x = x;  //controls the rect's x coordinate
        Message_rect.y = y; // controls the rect's y coordinte
        Message_rect.w = w; // controls the width of the rect
        Message_rect.h = h; // controls the height of the rect

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
        /* SDL_RenderPresent(renderer); */

        // Don't forget to free your surface and texture
        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
        TTF_CloseFont(Sans);
    }
}
