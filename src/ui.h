#ifndef UI_H_
#define UI_H_

/*************************
 * INCLUDES
 *************************/
#include <stdint.h>
#include "player_entity.h"

/*************************
 * Prototypes
 *************************/
void render_score(player_t player, uint32_t x, uint32_t y);

void render_message(const char *message, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

#endif // UI_H_
