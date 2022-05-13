#ifndef PLAYER_ENTITY_H_
#define PLAYER_ENTITY_H_

/*************************
 * INCLUDES
 *************************/
#include "types.h"

/*************************
 * GLOBAL Variables
 *************************/
extern player_t player1;
extern player_t player2;

/*************************
 * Prototypes
 *************************/
player_t create_player(void);

void update_players(float elapsed);

void handle_players_bounds(void);

void handle_players_inputs(float elapsed);

void render_players(void);

void move_player_up(player_t *player, float ticks);

void move_player_down(player_t *player, float ticks);

void restart_player_position(player_t *player);


#endif // PLAYER_ENTITY_H_
