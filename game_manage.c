#include "globals.h"
#include "room.h"
#include "client.h"
#include "game_manage.h"
#include "pong_logic/ball.h"
#include "pong_logic/paddle.h"
#include "pong_logic/init.h"
#include <stdio.h>

extern Room rooms[MAX_ROOMS];
extern client clientList[MAX_ROOMS + 1];
extern int sockfd;
int room_id = 0;
void start_pong(int new_room_id, int sockfd)
{
    room_id = new_room_id;
    printf("Starting game in room %d\n", room_id);
    init_game(&rooms[room_id].ball, rooms[room_id].paddle, SCREEN_HEIGHT, SCREEN_WIDTH);
    send_ball();
    send_paddle1();
    send_paddle2();
    send_score();
}

void send_ball()
{
    char ball[BUF_SIZE];
    sprintf(ball, "BALL: %d %d %d %d", rooms[room_id].ball.x, rooms[room_id].ball.y, rooms[room_id].ball.dx, rooms[room_id].ball.dy);
    broadcast_room(room_id, ball, sockfd);
}

void send_paddle1(){
    char paddle1[BUF_SIZE];
    sprintf(paddle1, "PADDLE1: %d %d", rooms[room_id].paddle[0].x, rooms[room_id].paddle[0].y);
    broadcast_room(room_id, paddle1, sockfd);
}

void send_paddle2(){
    char paddle2[BUF_SIZE];
    sprintf(paddle2, "PADDLE2: %d %d", rooms[room_id].paddle[1].x, rooms[room_id].paddle[1].y);
    broadcast_room(room_id, paddle2, sockfd);
}

void send_score(){
    char score[BUF_SIZE];
    sprintf(score, "SCORE: %d %d", rooms[room_id].score[0], rooms[room_id].score[1]);
    broadcast_room(room_id, score, sockfd);
}