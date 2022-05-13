#ifndef BALL_ENTITY_H_
#define BALL_ENTITY_H_

/*************************
 * INCLUDES
 *************************/
#include <stdint.h>
#include "types.h"

/*************************
 * GLOBAL Variables
 *************************/
extern bool should_restart_round;
extern uint32_t pause_timer;
extern ball_t ball;

/*************************
 * Prototypes
 *************************/
ball_t create_ball(int size);

void render_ball(const ball_t *ball);

void restart_ball_position(ball_t *ball);

void update_ball(ball_t *ball, float elapsed);

#endif // BALL_ENTITY_H_
