#include "ball.h"
#include "paddle.h"
#include "init.h"
#include "status_checkers.h"
// Función para inicializar una pelota
void init_ball(ball_t *ball, int x, int y, int w, int h, int dx, int dy)
{
    ball->x = x;
    ball->y = y;
    ball->w = w;
    ball->h = h;
    ball->dx = dx;
    ball->dy = dy;
}

// Función para mover una pelota
void move_ball(ball_t *ball, paddle_t paddle[2], int h, int w, int score[2]){
    ball->x += ball->dx;
    ball->y += ball->dy;

    if (ball->x < 0)
    {
        score[1] += 1;
        init_game(ball, paddle, h, w);
    }

    if (ball->x > w - 10)
    {
        score[0] += 1;
        init_game(ball, paddle, h, w);
    }

    if (ball->y < 0 || ball->y > h - 10)
    {
        ball->dy = -ball->dy;
    }

    // check for collision with the paddle
    int i;

    for (i = 0; i < 2; i++)
    {
        int c = check_collision(*ball, paddle[i]);

        // collision detected
        if (c == 1)
        {
            collision_handler(ball, paddle[i]);
        }
    }


}