/*************************
 * INCLUDES
 *************************/
#include "SDL2/SDL.h"
#include "game.h"
#include "config.h"
#include "ball_entity.h"
#include "player_entity.h"

/*************************
 * GLOBAL Variables
 *************************/
bool should_restart_round = false;
uint32_t pause_timer = 0;
ball_t ball = { 0 };

/*************************
 * Functions definitions
 *************************/
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
