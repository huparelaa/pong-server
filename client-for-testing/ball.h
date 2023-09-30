#include "paddle.h"
#ifndef BALL_H
#define BALL_H

// Definición de la estructura ball_t
typedef struct {
    int x, y; // posición en la pantalla
    int w, h; // ancho y alto de la pelota
    int dx, dy; // vector de movimiento
} ball_t;


#endif
