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
FILE *historialFile;


int main(int argc, char *argv[])
{
    // check if user provided a file name for the history
    if (argc != 2) {
        fprintf(stderr, "Uso incorrecto. Debe proporcionar un nombre de archivo de historial.\n");
        return 1;
    }

    char *historialFileName = malloc(strlen(argv[1]) + 5);
    strcpy(historialFileName, argv[1]);
    strcat(historialFileName, ".txt");

    // open file for appending
    historialFile = fopen(historialFileName, "a");
    
    if (historialFile == NULL) {
        perror("Error al abrir el archivo de historial");
        return 1;
    }

    bzero(request_buffer, BUF_SIZE);

    start_server(argv[1]);
    
    while (TRUE)
    {
        if((nbytes=listen_for_packets(request_buffer))==-1){
            perror("Error listening for packets");
            exit(1);
        }

        request_buffer[nbytes] = '\0'; // ensure that the string is null terminated
        fprintf(historialFile,"Received packages of %d bytes: %s\n", nbytes, request_buffer);
        printf("Received packages of %d bytes: %s\n", nbytes, request_buffer);

        // execute main program
        input_handler(request_buffer);
    }
    fclose(historialFile);
}