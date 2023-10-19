#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "protocole.h"
#include "globals.h"
#include "room.h"
#include "client.h"
#include "game_manage.h"

int sockfd;
int address_size = sizeof(struct sockaddr_in);
struct sockaddr_in server_addr;
struct sockaddr_in sender_addr;
char responseBuffer[BUF_SIZE + USERNAME_LEN];
extern Room rooms[MAX_ROOMS];
extern FILE *historialFile;

void start_server()
{
    // SOCK_DGRAM are datagram or connectionless sockets they use UDP and will not nessecarily arrive in order or arrive at all
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // returns socket file descriptor that can use send() and recv() for data transmission
    // last argumant protocol number given 0 will derive to protocol based on the 2nd argument type passed

    if (sockfd == SYSERR)
    { // socket() returns -1 on error
        close(sockfd);
        fprintf(stderr, "Failed to get socket file descriptor\n");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;                // address family we should use for this assignment
    server_addr.sin_port = htons(PORT);              // flips port argument to big endian and assigns to our port
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // binds socket to all the computers interfaces
    // INADDR_ANY is defined as 0 but is flipped to network long for consistency

    memset(&(server_addr.sin_zero), '\0', 8); // sets rest of sockaddr_in to 0's 8 bytes worth

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == SYSERR)
    {
        close(sockfd);
        fprintf(stderr, "Failed to bind on socket!\n");
        exit(EXIT_FAILURE);
    }
}

int listen_for_packets(char *request_buffer)
{
    return recvfrom(sockfd, request_buffer, BUF_SIZE - 1, 0, (struct sockaddr *)&sender_addr, (unsigned int *)&address_size);
}

// function to manage inputs
void input_handler(char *requestBuffer)
{
    bzero(responseBuffer, BUF_SIZE + USERNAME_LEN);
    char sender_name[USERNAME_LEN];
    // consigo el numero de la sala del cliente  que envio el mensaje
    int room_id = get_room_of_client(sender_addr);

    if (isConnected(sender_addr, sender_name))
    {
        userColor(sender_addr.sin_port, responseBuffer);
        strcat(responseBuffer, sender_name);
        if (strcmp(CLOSE, requestBuffer) == 0)
        {
            int previous_room_id = get_room_of_client(sender_addr);
            if (disconnectClient(sender_addr, previous_room_id) == OK)
            { // upon success of disconnect broadcast message to clients that user left

                // broadcast message to clients in previous room
                strcat(responseBuffer, RED " left the server" RESET "\n");
                broadcast_room(previous_room_id, responseBuffer, sockfd);
            }
        }
        else if (strcmp(EXIT, requestBuffer) == 0)
        {
            strcat(responseBuffer, RED " shutdown the server" RESET "\n");
            broadcast(sender_addr, TRUE, sockfd, responseBuffer);
            fprintf(historialFile, "Exiting Server\n");
            printf("Exiting Server\n");
            close(sockfd);
            exit(OK);
        }
        else if (strncmp(requestBuffer, ROOM_COMMAND, 5) == 0) // Los primeros 5 caracteres de requestBuffer son "/room"
        {
            join_room(sender_addr, requestBuffer, sockfd, responseBuffer);
        }
        else if (strcmp(SHOW_CLIENTS, requestBuffer) == 0)
        {
            sendClientList(sender_addr, sockfd, responseBuffer, MAX_ROOMS);
        }
        else if (strcmp(requestBuffer, START) == 0)
        {
            start_game(sockfd, sender_addr, responseBuffer);
        }
        else if (strncmp(requestBuffer, PADDLE1, 7) == 0)
        {
            broadcast(sender_addr, FALSE, sockfd, requestBuffer);
        }
        else if (strncmp(requestBuffer, PADDLE2, 7) == 0)
        {
            broadcast(sender_addr, FALSE, sockfd, requestBuffer);
        }
        else if (strncmp(requestBuffer, BALL, 5) == 0)
        {
            broadcast(sender_addr, FALSE, sockfd, requestBuffer);
        }
        else if (strncmp(requestBuffer, SCORE_SENDER_1, 8) == 0)
        {
            update_score(sockfd, sender_addr);
        }
        else if (strncmp(requestBuffer, SCORE_SENDER_2, 8) == 0)
        {
            update_score(sockfd, sender_addr);
        }
        else
        {
            strcat(responseBuffer, RESET);
            strcat(responseBuffer, USERNAMExMESSAGE); // inserts string between username and message to look nice
            strcat(responseBuffer, requestBuffer);
            fprintf(historialFile, "Message:[%s]\n", responseBuffer);
            printf("Message:[%s]\n", responseBuffer);
            // go through entire linked list and echo back the message to all clients connected with proper username of the sender
            broadcast(sender_addr, FALSE, sockfd, responseBuffer); // sends message to all except sender
        }
    }
    else
    {
        if (connectClient(sender_addr, requestBuffer, sockfd, responseBuffer, MAX_ROOMS) == OK)
        {
            userColor(sender_addr.sin_port, responseBuffer);
            strcat(responseBuffer, requestBuffer);
            strcat(responseBuffer, GREEN " connected" RESET "\n");
            broadcast(sender_addr, TRUE, sockfd, responseBuffer);
            sendClientList(sender_addr, sockfd, responseBuffer, MAX_ROOMS);
        }
    }
    bzero(responseBuffer, BUF_SIZE + USERNAME_LEN);
}
