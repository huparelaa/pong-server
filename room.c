#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "room.h"
#include "globals.h"

#include "client.h"

Room rooms[MAX_ROOMS];
client clientList[MAX_CLIENTS + 1];

int connectClient(struct sockaddr_in newClient, char *username, int sockfd, char responseBuffer[BUF_SIZE + USERNAME_LEN])
{
    printf("Attempting to connect client: %s\n", username);
    // clientList[last_position] will be the general and initial room
    client *element = &clientList[MAX_CLIENTS];

    while (element != NULL)
    {
        if (strcmp(element->username, username) == 0)
        {
            printf("Cannot connect client user already exists\n");
            strcpy(responseBuffer, "");
            strcat(responseBuffer, ERROR);
            if ((sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&newClient,
                        sizeof(struct sockaddr))) == SYSERR)
            {
                perror("sendto");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
            return SYSERR;
        }
        element = element->next;
    }

    element = &clientList[MAX_CLIENTS];
    while (element->next != NULL)
    {
        element = element->next;
    }
    element->next = malloc(sizeof(client));
    element = element->next;

    element->address = newClient;
    strncpy(element->username, username, USERNAME_LEN);
    element->next = NULL;
    printf("Client connected\n");
    return OK;
}

int disconnectClient(struct sockaddr_in oldClient)
{
    printf("Attempting to disconnect client\n");
    client *temp;
    client *element = &clientList[MAX_CLIENTS];

    while (element->next != NULL)
    {
        if (clientCompare(oldClient, element->next->address))
        {
            temp = element->next->next;
            free(element->next);
            element->next = temp;
            printf("Client disconnected\n");
            return OK;
        }
        element = element->next;
    }

    printf("Client was not disconnected properly\n");
    return SYSERR;
}

int isConnected(struct sockaddr_in newClient, char *sender_name)
{
    client *element = &clientList[MAX_CLIENTS];

    while (element != NULL)
    {
        if (clientCompare(element->address, newClient))
        {
            strncpy(sender_name, element->username, USERNAME_LEN);
            printf("Client is already connected\n");
            return TRUE;
        }
        element = element->next;
    }
    printf("Client is not already connected\n");
    return FALSE;
}

/* sends message to all clients except for the sender */
/* will send to all clients if second argument `global` is set to TRUE */
void broadcast(struct sockaddr_in sender, int global, int sockfd, char *responseBuffer)
{
    client *cli = clientList[MAX_CLIENTS].next; // client list iterator

    while (cli != NULL)
    {
        // if sender isn't the client send out the message // may need separate clientCompare function // use username for comparisons so noone can connect with the same username
        if (clientCompare(sender, cli->address) == FALSE || global)
        {
            printf("Sending message to %s\n", cli->username);
            if ((sendto(sockfd, responseBuffer, strlen(responseBuffer), 0,
                (struct sockaddr *)&cli->address, sizeof(struct sockaddr))) == SYSERR)
            {
                perror("sendto");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
        }
        cli = cli->next;
    }
}

void sendClientList(struct sockaddr_in sender, int sockfd, char *responseBuffer) {
    client *cli = clientList[MAX_CLIENTS].next;

    while(cli != NULL) {
        if(clientCompare(sender, cli->address) == FALSE) {
            strcpy(responseBuffer, "");
            userColor(cli->address.sin_port, responseBuffer);
            strcat(responseBuffer, cli->username);
            strcat(responseBuffer, RESET "\n");
            if((sendto
                (sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *) &sender,
                 sizeof(struct sockaddr))) == SYSERR) {

                perror("sendto");
                close(sockfd);
                exit(EXIT_FAILURE);
            }
        }
        cli = cli->next;
    }
}

int is_room_full(int room_id)
{
    if (rooms[room_id].player_count == MAX_CLIENTS)
    {
        return TRUE;
    }
    return FALSE;
}

void join_room(int room_id, client *c)
{
    if (is_room_full(room_id))
    {
        printf("Room is full\n");
        print_available_rooms();
        return;
    }
    rooms[room_id].players[rooms[room_id].player_count] = *c;
    rooms[room_id].player_count++;
}

void print_clients_in_room(int room_id)
{
    printf("Clients in room %d:\n", room_id);
    for (int i = 0; i < rooms[room_id].player_count; i++)
    {
        printf("%s\n", rooms[room_id].players[i].username);
    }
}

void print_available_rooms()
{
    printf("Available rooms:\n");
    for (int i = 0; i < MAX_ROOMS; i++)
    {
        if (rooms[i].player_count < MAX_CLIENTS)
        {
            printf("Room %d\n", i);
        }
    }
}