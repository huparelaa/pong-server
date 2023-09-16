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

int client_socket;
char buffer[BUFFER_SIZE];

void *receiveMessage(void *arg) {
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Servidor: %s", buffer);
    }
    return NULL;
}

int main() {
    struct sockaddr_in server_addr;

    // Crear el socket del cliente
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error al crear el socket del cliente");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error en la conexión");
        exit(1);
    }

    printf("Conectado al servidor de Pong\n");

    pthread_t receive_thread;
    pthread_create(&receive_thread, NULL, receiveMessage, NULL);

    // Envía mensajes al servidor
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        printf("Cliente: ");
        fgets(buffer, sizeof(buffer), stdin); // Lee la entrada del cliente desde la consola
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    pthread_join(receive_thread, NULL);

    return 0;
}
