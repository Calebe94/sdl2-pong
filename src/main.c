/*************************
 * INCLUDES
 *************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL.h>
#include <SDL_ttf.h>
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#endif
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
#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(game_loop, 0, 1);
#endif
#ifndef __EMSCRIPTEN__
        game_loop();
#endif
    }

    game_close();
    return 0;
}
