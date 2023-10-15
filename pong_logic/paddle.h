#ifndef PADDLE_H
#define PADDLE_H

// Definición de la estructura paddle_t
typedef struct {
    int x, y; // posición en la pantalla
    int w, h; // ancho y alto de la paleta
    int dy; // vector de movimiento
} paddle_t;

// Funciones relacionadas con la paleta
void init_paddle(paddle_t *paddle, int x, int y, int w, int h, int dy);

#endif
