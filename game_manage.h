#ifndef GAME_MANAGE_H
#define GAME_MANAGE_H

void start_pong(int new_room_id, int sockfd);
void send_ball();
void send_paddle1();
void send_paddle2();
void send_score();
void send_player_number();
void update_score(int room_id, int player);
void restart_ball_and_paddle_position();
void close_game();

#endif