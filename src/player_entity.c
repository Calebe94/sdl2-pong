/*************************
 * INCLUDES
 *************************/
#include "config.h"
#include "game.h"
#include "types.h"
#include "player_entity.h"

/*************************
 * GLOBAL Variables
 *************************/
player_t player1 = { 0 };
player_t player2 = { 0 };

/*************************
 * Functions definitions
 *************************/
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
    if (keyboard_state[SDL_SCANCODE_UP])
    {
        move_player_up(&player1, elapsed);
    }
    if (keyboard_state[SDL_SCANCODE_DOWN])
    {
        move_player_down(&player1, elapsed);
    }
    /* if (keyboard_state[SDL_SCANCODE_UP]) */
    /* { */
    /*     move_player_up(&player2, elapsed); */
    /* } */
    /* if (keyboard_state[SDL_SCANCODE_DOWN]) */
    /* { */
    /*     move_player_down(&player2, elapsed); */
    /* } */
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
