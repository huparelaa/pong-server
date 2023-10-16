#ifndef ROOM_H
#define ROOM_H
#include "client.h"
#include "globals.h"
#include "pong_logic/ball.h"
#include "pong_logic/paddle.h"
typedef struct {
    int room_id;
    int player_count;
    int status;
    ball_t ball;
    paddle_t paddle[2];
    //score
    int score[2];
    client players[MAX_CLIENTS];
    int game_started;
} Room;

void broadcast(struct sockaddr_in sender, int global, int sockfd, char *responseBuffer);
int connectClient(struct sockaddr_in newClient, char *username, int sockfd, char responseBuffer[BUF_SIZE+USERNAME_LEN], int room_id);
int isConnected(struct sockaddr_in newClient, char *sender_name);
int disconnectClient(struct sockaddr_in oldClient, int room_id);
void sendClientList(struct sockaddr_in sender, int sockfd, char *responseBuffer, int room_id);

int is_room_full(int room_id);
int check_valid_room(int room_id);
void join_room(struct sockaddr_in client, char *requestBuffer, int sockfd, char responseBuffer[BUF_SIZE + USERNAME_LEN]);
int get_room_of_client(struct sockaddr_in client_for_search);
client *getClient(struct sockaddr_in client_address);
void broadcast_room(int room_id, char *responseBuffer, int sockfd);

//game
void start_game(int sockfd, struct sockaddr_in client_address, char *responseBuffer);

#endif // ROOM_H