#include "globals.h"
#include "room.h"
#include "client.h"
#include "game_manage.h"
#include "pong_logic/ball.h"
#include "pong_logic/paddle.h"
#include "pong_logic/init.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern Room rooms[MAX_ROOMS];
extern client clientList[MAX_ROOMS + 1];
extern FILE *historialFile;

void start_pong(int new_room_id, int sockfd)
{
    int room_id = new_room_id;
    fprintf(historialFile, "Starting game in room %d\n", room_id);
    printf("Starting game in room %d\n", room_id);
    init_game(&rooms[room_id].ball, rooms[room_id].paddle, SCREEN_HEIGHT, SCREEN_WIDTH);
    send_ball(room_id, sockfd);
    send_paddle1(room_id, sockfd);
    send_paddle2(room_id, sockfd);
    send_score(room_id, sockfd);
    send_player_number(room_id, sockfd);
}

void send_restart_game(int room_id, int sockfd)
{
    char restart_game[BUF_SIZE];
    sprintf(restart_game, "RESTART");
    printf("Restarting game in room %d\n", room_id);
    broadcast_room(room_id, restart_game, sockfd);
}

void send_ball(int room_id, int sockfd)
{
    char ball[BUF_SIZE];
    sprintf(ball, "BALL: %d %d %d %d", rooms[room_id].ball.x, rooms[room_id].ball.y, rooms[room_id].ball.dx, rooms[room_id].ball.dy);
    broadcast_room(room_id, ball, sockfd);
}

void send_paddle1(int room_id, int sockfd)
{
    char paddle1[BUF_SIZE];
    sprintf(paddle1, "PADDLE1: %d %d", rooms[room_id].paddle[0].x, rooms[room_id].paddle[0].y);
    broadcast_room(room_id, paddle1, sockfd);
}

void send_paddle2(int room_id, int sockfd)
{
    char paddle2[BUF_SIZE];
    sprintf(paddle2, "PADDLE2: %d %d", rooms[room_id].paddle[1].x, rooms[room_id].paddle[1].y);
    broadcast_room(room_id, paddle2, sockfd);
}

void send_score(int room_id, int sockfd)
{
    char score[BUF_SIZE];
    sprintf(score, "SCORE: %d %d", rooms[room_id].score[0], rooms[room_id].score[1]);
    broadcast_room(room_id, score, sockfd);
}

void update_score(int sockfd, struct sockaddr_in client_addr)
{
    int p_number = !(getClient(client_addr)->player_number-1); // coversion to binary and change to opponent position
    p_number = p_number + 1; // conversion to decimal
    int room_id = get_room_of_client(client_addr);
    printf("Player %d scored in room %d\n", p_number, room_id);
    rooms[room_id].score[p_number - 1] += 1;
    restart_ball_and_paddle_position(room_id, sockfd);
    // verificamos si es el máximo score
    if (rooms[room_id].score[p_number - 1] == MAX_SCORE)
    {
        // enviamos el score
        send_score(room_id, sockfd);
        // enviamos el ganador
        char winner[BUF_SIZE];
        sprintf(winner, "WINNER: %d", p_number);
        broadcast_room(room_id, winner, sockfd);

        // print winner
        printf("Winner: %d in room %d\n", p_number, room_id);

        // reiniciamos el juego
        rooms[room_id].game_started = FALSE;
        rooms[room_id].score[0] = 0;
        rooms[room_id].score[1] = 0;
        restart_ball_and_paddle_position(room_id, sockfd);
        return;
    }
    send_score(room_id, sockfd);
}

void restart_ball_and_paddle_position(int room_id, int sockfd)
{
    init_game(&rooms[room_id].ball, rooms[room_id].paddle, SCREEN_HEIGHT, SCREEN_WIDTH);
    send_restart_game(room_id, sockfd);
    send_ball(room_id, sockfd);
    send_paddle1(room_id, sockfd);
    send_paddle2(room_id, sockfd);
}

void send_player_number(int room_id, int sockfd)
{
    char player_number[BUF_SIZE];

    // ponemos el valor en 1
    sprintf(player_number, "PLAYER: %d", 1);

    client *cli = clientList[room_id].next;
    while (cli != NULL)
    {
        if ((sendto(sockfd, player_number, strlen(player_number), 0, (struct sockaddr *)&cli->address,
                    sizeof(struct sockaddr))) == SYSERR)
        {
            perror("sendto");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        // ponemos el valor en 2
        sprintf(player_number, "PLAYER: %d", 2);
        cli = cli->next;
    }
}