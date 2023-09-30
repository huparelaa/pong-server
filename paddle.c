#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "paddle.h"

// Función para inicializar una paleta
void init_paddle(paddle_t *paddle, int x, int y, int w, int h, int dy)
{
    paddle->x = x;
    paddle->y = y;
    paddle->w = w;
    paddle->h = h;
    paddle->dy = dy;
}

// Función para mover una paleta
void move_paddle(paddle_t *paddle, int height, int d)
{
    if (d == 0)
    {
        if (paddle->y >= height - paddle->h)
        {
            paddle->y = height - paddle->h;
        }
        else
        {
            paddle->y += 5;
        }
    }
    if (d == 1)
    {
        if (paddle->y <= 0)
        {
            paddle->y = 0;
        }
        else
        {
            paddle->y -= 5;
        }
    }
}
