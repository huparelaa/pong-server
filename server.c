#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int server_socket, new_socket;
struct sockaddr_in server_addr, new_addr;
socklen_t addr_size;
char buffer[BUFFER_SIZE];

void *sendMessage(void *arg) {
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        printf("Servidor: ");
        fgets(buffer, sizeof(buffer), stdin); // Lee la entrada del servidor desde la consola
        send(new_socket, buffer, strlen(buffer), 0);
    }
    return NULL;
}

int main() {
    // Crear el socket del servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error al crear el socket del servidor");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Vincular el socket a la dirección y el puerto
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en la vinculación");
        exit(1);
    }

    // Escuchar conexiones entrantes
    if (listen(server_socket, 10) == 0) {
        printf("Escuchando... \n");
    } else {
        perror("Error en la escucha");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_socket = accept(server_socket, (struct sockaddr*)&new_addr, &addr_size); // Aceptar la conexión entrante

    pthread_t send_thread;
    pthread_create(&send_thread, NULL, sendMessage, NULL);

    // Inicio del juego de Pong
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(new_socket, buffer, BUFFER_SIZE, 0); // Recibir mensaje del cliente
        printf("Cliente: %s", buffer);
    }

    close(new_socket);
    close(server_socket);

    pthread_join(send_thread, NULL);

    return 0;
}
