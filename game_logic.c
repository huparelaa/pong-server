#include <stdlib.h>
#include <stdio.h>
#include "ball.h"
#include "paddle.h"
#include "status_checkers.h"
#include "init.h"
#include "protocole.h"
#include "globals.h"

#include "string.h"
#include <netinet/in.h> //para la estructura de la direccion
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define WIDTH 640
#define HEIGHT 480

static ball_t ball;
static paddle_t paddle[2];
int score[] = {0, 0};

int main(int argc, char *argv[])
{
    
    char *ip = IP;
    int port = PORT;

    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[1024];
    socklen_t addr_size;
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        perror("[-]socket error");
        exit(1);
    }

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("[-]bind error");
        exit(1);
    }

    int sleep = 0;
    int quit = 0;
    int state = 0;
    int r = 0;

    // Initialize the ball position data.
    init_game(&ball, paddle, HEIGHT, WIDTH);
    printf("Game initialized\n");
    while (quit == 0)
    {
        //refactorizar en funciones 
        //---------------------------------------
        bzero(buffer, 1024);
        addr_size = sizeof(client_addr);
        recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&client_addr, &addr_size);
        printf("[+]Data recv: %s\n", buffer);

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
        else if (strcmp(buffer, "START") == 0)
        {
            state = 1;
        }

        game_status game;
        game.ball = ball;
        game.paddles[0] = paddle[0];
        game.paddles[1] = paddle[1];
        game.score[0] = score[0];
        game.score[1] = score[1];
        game.status = state;
        game.r = r;
        printf("Sending game status\n");
        sendto(sockfd, &game, sizeof(game), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
        //---------------------------------------
        // Mostramos el estado del juego
        printf("Ball: x: %d, y: %d\n", ball.x, ball.y);
        printf("Paddle 1: x: %d, y: %d\n", paddle[0].x, paddle[0].y);
        printf("Paddle 2: x: %d, y: %d\n", paddle[1].x, paddle[1].y);
        printf("Score: %d - %d\n", score[0], score[1]);
        printf("Status: %d\n", state);
        printf("R: %d\n", r);

        if (state == 1)
        {
            r = check_score(MAX_SCORE, score);
            if (r == 1)
            {
                state = 2;
            }
            else if (r == 2)
            {
                state = 2;
            }
            move_ball(&ball, paddle, HEIGHT, WIDTH, score);
        }
        else if (state == 2)
        {
            quit = 1;
        }
    }
}
