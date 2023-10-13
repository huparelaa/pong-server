#include <netinet/in.h>
#include "globals.h"

#ifndef CLIENT_H
#define CLIENT_H

typedef struct client
{
    struct sockaddr_in address;  // will hold the client's address information
    char username[USERNAME_LEN]; // will hold the clients username and will be initialized when each client starts and sends it's username and a special code informative of joining server
    struct client *next;
    int room_id;
} client;

int clientCompare(struct sockaddr_in client1, struct sockaddr_in client2);
void userColor(int n, char *response_buffer);

#endif // CLIENT_H