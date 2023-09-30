#include <stdlib.h>
#include <stdio.h>
#include "ball.h"
#include "paddle.h"
int check_score(int MAX_SCORE, int score[2])
{
    for (int i = 0; i < 2; i++)
    {
        if (score[i] == MAX_SCORE)
        {

            // reset scores
            score[0] = 0;
            score[1] = 0;

            // return 1 if player 1 score @ limit
            if (i == 0)
            {

                return 1;

                // return 2 if player 2 score is @ limit
            }
            else
            {

                return 2;
            }
        }
    }

    // return 0 if no one has reached a score of 10 yet
    return 0;
}
// if return 1 collision detected else return 0
int check_collision(ball_t a, paddle_t b)
{
    int left_a, left_b;
    int right_a, right_b;
    int top_a, top_b;
    int bottom_a, bottom_b;

    left_a = a.x;
    right_a = a.x + a.w;
    top_a = a.y;
    bottom_a = a.y + a.h;

    left_b = b.x;
    right_b = b.x + b.w;
    top_b = b.y;
    bottom_b = b.y + b.h;

    if (left_a > right_b)
    {
        return 0;
    }

    if (right_a < left_b)
    {
        return 0;
    }

    if (top_a > bottom_b)
    {
        return 0;
    }

    if (bottom_a < top_b)
    {
        return 0;
    }

    return 1;
}

void collision_handler(ball_t *ball, paddle_t paddle)
{
    // ball moving left
    if (ball->dx < 0)
    {

        ball->dx -= 1;

        // ball moving right
    }
    else
    {

        ball->dx += 1;
    }

    // change ball direction
    ball->dx = -ball->dx;

    // change ball angle based on where on the paddle it hit
    int hit_pos = (paddle.y + paddle.h) - ball->y;

    if (hit_pos >= 0 && hit_pos < 7)
    {
        ball->dy = 4;
    }

    if (hit_pos >= 7 && hit_pos < 14)
    {
        ball->dy = 3;
    }

    if (hit_pos >= 14 && hit_pos < 21)
    {
        ball->dy = 2;
    }

    if (hit_pos >= 21 && hit_pos < 28)
    {
        ball->dy = 1;
    }

    if (hit_pos >= 28 && hit_pos < 32)
    {
        ball->dy = 0;
    }

    if (hit_pos >= 32 && hit_pos < 39)
    {
        ball->dy = -1;
    }

    if (hit_pos >= 39 && hit_pos < 46)
    {
        ball->dy = -2;
    }

    if (hit_pos >= 46 && hit_pos < 53)
    {
        ball->dy = -3;
    }

    if (hit_pos >= 53 && hit_pos <= 60)
    {
        ball->dy = -4;
    }

    // ball moving right
    if (ball->dx > 0)
    {

        // teleport ball to avoid mutli collision glitch
        if (ball->x < 30)
        {

            ball->x = 30;
        }

        // ball moving left
    }
    else
    {

        // teleport ball to avoid mutli collision glitch
        if (ball->x > 600)
        {

            ball->x = 600;
        }
    }
}
