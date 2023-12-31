#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "room.h"
#include "globals.h"
#include "game_manage.h"
#include "client.h"

extern FILE *historialFile;

Room rooms[MAX_ROOMS];
client clientList[MAX_ROOMS + 1];

int connectClient(struct sockaddr_in newClient, char *username, int sockfd, char responseBuffer[BUF_SIZE + USERNAME_LEN], int room_id)
{
    fprintf(historialFile, "Attempting to connect client: %s\n", username);
    printf("Attempting to connect client: %s\n", username);
    // clientList[last_position] will be the general and initial room
    client *element = &clientList[room_id];

    while (element != NULL)
    {
        if (strcmp(element->username, username) == 0)
        {
            fprintf(historialFile, "Cannot connect client user already exists\n");
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

    element = &clientList[room_id];
    while (element->next != NULL)
    {
        element = element->next;
    }
    element->next = malloc(sizeof(client));
    element = element->next;

    element->address = newClient;
    strncpy(element->username, username, USERNAME_LEN);
    element->room_id = room_id;
    element->next = NULL;
    rooms[room_id].player_count++;
    element->player_number = rooms[room_id].player_count;
    fprintf(historialFile, "Client connected\n");
    printf("Client connected\n");
    return OK;
}

int disconnectClient(struct sockaddr_in oldClient, int room_id)
{
    fprintf(historialFile, "Attempting to disconnect client\n");
    printf("Attempting to disconnect client\n");
    client *temp;
    client *element = &clientList[room_id];

    while (element->next != NULL)
    {
        if (clientCompare(oldClient, element->next->address))
        {
            temp = element->next->next;
            free(element->next);
            element->next = temp;
            fprintf(historialFile, "Client disconnected\n");
            printf("Client disconnected\n");
            rooms[room_id].player_count--;
            return OK;
        }
        element = element->next;
    }
    fprintf(historialFile, "Client was not disconnected properly\n");
    printf("Client was not disconnected properly\n");
    return SYSERR;
}

int isConnected(struct sockaddr_in newClient, char *sender_name)
{
    for (int i = 0; i <= MAX_ROOMS; i++)
    {
        client *element = &clientList[i];

        while (element != NULL)
        {
            if (clientCompare(element->address, newClient))
            {
                strncpy(sender_name, element->username, USERNAME_LEN);
                fprintf(historialFile, "Client is already connected\n");
                printf("Client is already connected\n");
                return TRUE;
            }
            element = element->next;
        }
    }
    fprintf(historialFile, "Client is not already connected\n");
    printf("Client is not already connected\n");
    return FALSE;
}

/* sends message to all clients except for the sender */
/* will send to all clients if second argument `global` is set to TRUE */
void broadcast(struct sockaddr_in sender, int global, int sockfd, char *responseBuffer)
{
    int room_id = get_room_of_client(sender);
    fprintf(historialFile, "Broadcasting message to room %d\n", room_id);
    printf("Broadcasting message to room %d\n", room_id);
    client *cli = clientList[room_id].next; // client list iterator

    while (cli != NULL)
    {
        // if sender isn't the client send out the message // may need separate clientCompare function // use username for comparisons so noone can connect with the same username
        if (clientCompare(sender, cli->address) == FALSE || global)
        {
            fprintf(historialFile, "Sending message to %s\n", cli->username);
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

void sendClientList(struct sockaddr_in sender, int sockfd, char *responseBuffer, int room_id)
{
    client *cli = clientList[room_id].next;

    while (cli != NULL)
    {
        if (clientCompare(sender, cli->address) == FALSE)
        {
            strcpy(responseBuffer, "");
            userColor(cli->address.sin_port, responseBuffer);
            strcat(responseBuffer, cli->username);
            strcat(responseBuffer, RESET "\n");
            if ((sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&sender,
                        sizeof(struct sockaddr))) == SYSERR)
            {

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
        fprintf(historialFile, "Room is full\n");
        printf("Room is full\n");
        return TRUE;
    }
    return FALSE;
}

void join_room(struct sockaddr_in newClient, char *requestBuffer, int sockfd, char responseBuffer[BUF_SIZE + USERNAME_LEN])
{
    int new_room_id = atoi(requestBuffer + 5); // extrae el número de la sala de requestBuffer
    client *cli = getClient(newClient);
    char room_id_str[20]; // Un búfer para almacenar la cadena
    sprintf(room_id_str, "%d", new_room_id);

    int previous_room_id = cli->room_id;
    if (check_valid_room(new_room_id) == SYSERR)
    {
        strcat(responseBuffer, RED " invalid room number" RESET "\n");
        sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&newClient,
               sizeof(struct sockaddr));
    }
    else if (is_room_full(new_room_id))
    {
        strcat(responseBuffer, RED " room is full" RESET "\n");
        sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&newClient,
               sizeof(struct sockaddr));
    }
    else
    {
        strcat(responseBuffer, GREEN " you joined to the room ");
        strcat(responseBuffer,  room_id_str);
        strcat(responseBuffer, RESET "\n");
        sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&newClient,
               sizeof(struct sockaddr));

        //clear te response buffer
        strcpy(responseBuffer, "");
        
        // disconnect client from previous room
        disconnectClient(newClient, previous_room_id);
        // broadcast message to clients in previous room
        strcat(responseBuffer, RED " left the room" RESET "\n");
        broadcast_room(previous_room_id, responseBuffer, sockfd);

        // connect client to new room
        connectClient(newClient, cli->username, sockfd, responseBuffer, new_room_id);
        // broadcast message to clients in new room
        strcpy(responseBuffer, "");
        userColor(cli->address.sin_port, responseBuffer);
        strcat(responseBuffer, cli->username);
        strcat(responseBuffer, RESET);
        strcat(responseBuffer, GREEN " joined to the room" RESET "\n");
        broadcast(newClient, FALSE, sockfd, responseBuffer);
    }
}

int check_valid_room(int room_id)
{
    if (room_id <= 0 || room_id >= MAX_ROOMS)
    {
        return SYSERR;
    }
    return OK;
}

client *getClient(struct sockaddr_in client_address)
{
    client *cli = clientList[get_room_of_client(client_address)].next;

    while (cli != NULL)
    {
        if (clientCompare(cli->address, client_address))
        {
            return cli;
        }
        cli = cli->next;
    }
    return NULL;
}

int get_room_of_client(struct sockaddr_in client_for_search)
{
    for (int i = 0; i <= MAX_ROOMS; i++)
    {
        client *element = &clientList[i];

        while (element != NULL)
        {
            if (clientCompare(element->address, client_for_search))
            {
                return i;
            }
            element = element->next;
        }
    }
    return SYSERR;
}

void broadcast_room(int room_id, char *responseBuffer, int sockfd)
{
    client *cli = clientList[room_id].next;

    while (cli != NULL)
    {
        if ((sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&cli->address,
                    sizeof(struct sockaddr))) == SYSERR)
        {
            perror("sendto");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        cli = cli->next;
    }
}

void start_game(int sockfd, struct sockaddr_in client_address, char *responseBuffer)
{
    int room_id = get_room_of_client(client_address);

    if (rooms[room_id].game_started == TRUE)
    {
        strcpy(responseBuffer, "");
        strcat(responseBuffer, RED " game already started" RESET "\n");
        sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&client_address,
               sizeof(struct sockaddr));
        return;
    }

    if(room_id == MAX_ROOMS){
        // cannot start game in general room
        strcpy(responseBuffer, "");
        strcat(responseBuffer, RED " cannot start game in general room" RESET "\n");
        sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&client_address,
               sizeof(struct sockaddr));
        return;
               
    }
    if (rooms[room_id].player_count == MAX_CLIENTS)
    {
        strcpy(responseBuffer, "game started\n");
        rooms[room_id].game_started = TRUE;
        // strcat(responseBuffer, GREEN " game started" RESET "\n");
        broadcast_room(room_id, responseBuffer, sockfd);
        start_pong(room_id, sockfd);
    }
    else
    {
        strcpy(responseBuffer, "");
        strcat(responseBuffer, RED " not enough players" RESET "\n");
        sendto(sockfd, responseBuffer, strlen(responseBuffer), 0, (struct sockaddr *)&client_address,
               sizeof(struct sockaddr));
    }
}