#ifndef ROOM_H
#define ROOM_H
#include "client.h"
#include "globals.h"

typedef struct {
    int room_id;
    int player_count;
    int status;
    client players[MAX_CLIENTS];
} Room;

void broadcast(struct sockaddr_in sender, int global, int sockfd, char *responseBuffer);
int connectClient(struct sockaddr_in newClient, char *username, int sockfd, char responseBuffer[BUF_SIZE+USERNAME_LEN]);
int isConnected(struct sockaddr_in newClient, char *sender_name);
int disconnectClient(struct sockaddr_in oldClient);
void sendClientList(struct sockaddr_in sender, int sockfd, char *responseBuffer);

int is_room_full(int room_id);
void join_room(int room_id, client *c);
void print_clients_in_room(int room_id);
void print_available_rooms();
#endif // ROOM_H