/*************************
 * INCLUDES
 *************************/

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <SDL.h>
#include <SDL_ttf.h>
#else
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#endif
#include "config.h"
#include "game.h"
#include "ui.h"

/*************************
 * Functions definitions
 *************************/
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
