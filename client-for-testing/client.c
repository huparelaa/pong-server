#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "game_status.h"
#include "ball.h"
#include "paddle.h"
#include <ncurses.h> //Para leer teclas sin esperar Enter

int main(int argc, char **argv)
{

  if (argc != 2)
  {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
  }

  char *ip = "127.0.0.1";
  int port = atoi(argv[1]);

  int sockfd;
  struct sockaddr_in addr;
  char buffer[1024];

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip);

  // Inicializar ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  while (1)
  {

    int ch = getch();

    if (ch == KEY_UP)
    {
      strcpy(buffer, "UP1");
    }
    if (ch == KEY_DOWN)
    {
      strcpy(buffer, "DOWN1");
    }
    if (ch == 'p')
    {
      strcpy(buffer, "START");
    }
    if (ch == 'q')
    {
      strcpy(buffer, "NONE");
    }

    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, sizeof(addr));
    printf("[+]Data send: %s\n", buffer);

    game_status game_status;
    socklen_t addr_size = sizeof(addr);
    recvfrom(sockfd, &game_status, sizeof(game_status), 0, (struct sockaddr *)&addr, &addr_size);
    // recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size);
    printf("Ball: x: %d, y: %d\n", game_status.ball.x, game_status.ball.y);
    printf("Paddle 1: x: %d, y: %d\n", game_status.paddles[0].x, game_status.paddles[0].y);
    printf("Paddle 2: x: %d, y: %d\n", game_status.paddles[1].x, game_status.paddles[1].y);
    printf("Score: %d - %d\n", game_status.score[0], game_status.score[1]);
    printf("Status: %d\n", game_status.status);
    printf("R: %d\n", game_status.r);
  }

  return 0;
}
