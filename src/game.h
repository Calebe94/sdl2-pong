#ifndef GAME_H_
#define GAME_H_
/*************************
 * INCLUDES
 *************************/
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

/*************************
 * GLOBAL Variables
 *************************/
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern bool playing;
extern uint32_t rounds;
extern bool run_game;

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

bool coin_flip(void);

void handle_colisions(void);

void show_controls();

void show_scores();
#endif // GAME_H_
