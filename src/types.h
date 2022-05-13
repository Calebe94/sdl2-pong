#ifndef TYPES_H_
#define TYPES_H_

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


#endif // TYPES_H_
