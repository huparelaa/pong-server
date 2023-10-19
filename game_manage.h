#ifndef GAME_MANAGE_H
#define GAME_MANAGE_H

void start_pong(int new_room_id, int sockfd);
void send_ball(int room_id, int sockfd);
void send_paddle1(int room_id, int sockfd);
void send_paddle2(int room_id, int sockfd);
void send_score(int room_id, int sockfd);
void send_player_number(int room_id, int sockfd);
void update_score(int sockfd, struct sockaddr_in client_addr);
void restart_ball_and_paddle_position(int room_id, int sockfd);
void send_restart_game(int room_id, int sockfd);

#endif