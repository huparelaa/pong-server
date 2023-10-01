#ifndef GAME_STATUS_H
#define GAME_STATUS_H
#include "ball.h"
#include "paddle.h"

typedef struct
{
    ball_t ball;
    paddle_t paddles[2];
    int score[2];
    int status;
    int r;  //determines the winner
    int number_of_players;

} game_status;

#endif