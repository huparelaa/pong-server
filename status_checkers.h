#include "ball.h"
#include "paddle.h"

#ifndef STATUS_CHECKERS_H
#define STATUS_CHECKERS_H

int check_score(int MAX_SCORE, int score[2]);

int check_collision(ball_t a, paddle_t b);

void collision_handler(ball_t *ball, paddle_t paddle);
#endif