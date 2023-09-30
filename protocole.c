#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h> //para la estructura de la direccion
#include <string.h>
#include "game_status.h"
//devuelve el estado del juego
void send_game_status(ball_t *ball, paddle_t paddle[2], int score[2], int status, int r, int sockfd, struct sockaddr_in client_addr, socklen_t addr_size)
{
    game_status game;
    game.ball = *ball;
    game.paddles[0] = paddle[0];
    game.paddles[1] = paddle[1];
    game.score[0] = score[0];
    game.score[1] = score[1];
    game.status = status;
    game.r = r;
    printf("Sending game status\n");
    sendto(sockfd, &game, sizeof(game), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
}
// Función para recibir el input del cliente y mover la paleta si es necesario
void rcv_input(int sockfd, struct sockaddr_in client_addr, socklen_t addr_size, char buffer[1024], paddle_t paddle[2], int HEIGHT, int *state)
{
    recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_size);
    printf("Received: %s\n", buffer);
    if (strcmp(buffer, "UP1") == 0)
    {
        move_paddle(&paddle[0], HEIGHT, 1);
    }
    else if (strcmp(buffer, "DOWN1") == 0)
    {
        move_paddle(&paddle[0], HEIGHT, 0);
    }
    else if (strcmp(buffer, "UP2") == 0)
    {
        move_paddle(&paddle[1], HEIGHT, 1);
    }
    else if (strcmp(buffer, "DOWN2") == 0)
    {
        move_paddle(&paddle[1], HEIGHT, 0);
    }
    else if(strcmp(buffer, "START") == 0){
        *state = 1;
    }
}