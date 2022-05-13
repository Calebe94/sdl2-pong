/*************************
 * INCLUDES
 *************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "config.h"
#include "types.h"
#include "ball_entity.h"
#include "player_entity.h"
#include "game.h"

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
