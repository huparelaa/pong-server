#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "server.h"
#include "client.h"
#include "room.h"
#include "protocole.h"

int nbytes;
char request_buffer[BUF_SIZE];
char sender_name[USERNAME_LEN];


int main()
{
    bzero(request_buffer, BUF_SIZE);

    start_server();
    
    while (TRUE)
    {
        if((nbytes=listen_for_packets(request_buffer))==-1){
            perror("Error listening for packets");
            exit(1);
        }

        request_buffer[nbytes] = '\0'; // ensure that the string is null terminated
        printf("Received packages of %d bytes: %s\n", nbytes, request_buffer);

        // execute main program
        input_handler(request_buffer);
    }
}