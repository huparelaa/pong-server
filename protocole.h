#ifndef PROTOCOLE_H
#define PROTOCOLE_H
#include "ball.h"
#include "paddle.h"
#include "game_status.h"
#include <netinet/in.h> //para la estructura de la direccion

void send_game_status(ball_t *ball, paddle_t paddle[2], int score[2], int status, int r, int sockfd, struct sockaddr_in client_addr, socklen_t addr_size);

void rcv_input(int sockfd, struct sockaddr_in client_addr, socklen_t addr_size, char buffer[1024], paddle_t paddle[2], int HEIGHT, int *state);

#endif