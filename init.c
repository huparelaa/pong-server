#include <stdio.h>
#include "ball.h"
#include "paddle.h"
#include "init.h"

void init_game(ball_t *ball, paddle_t paddle[2], int h, int w){

    ball->x = w / 2;
    ball->y = h / 2;
    ball->w = 10;
    ball->h = 10;
    ball->dy = 1;
    ball->dx = 1;



    paddle[0].x = 20;
    paddle[0].y = h / 2 - 50;
    paddle[0].w = 10;
    paddle[0].h = 50;

    paddle[1].x = w - 20 - 10;
    paddle[1].y = h / 2 - 50;
    paddle[1].w = 10;
    paddle[1].h = 50;

}