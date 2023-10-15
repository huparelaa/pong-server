#include "paddle.h"
#ifndef BALL_H
#define BALL_H

// Definición de la estructura ball_t
typedef struct {
    int x, y; // posición en la pantalla
    int w, h; // ancho y alto de la pelota
    int dx, dy; // vector de movimiento
} ball_t;

// Funciones relacionadas con la pelota
void init_ball(ball_t *ball, int x, int y, int w, int h, int dx, int dy);
/* This routine moves each ball by its motion vector. */
void move_ball(ball_t *ball, paddle_t paddle[2], int h, int w, int score[2]);

#endif
