#ifndef PROTOCOLE_H
#define PROTOCOLE_H
#include "globals.h"

int listen_for_packets(char *request_buffer);
void start_server();
void input_handler(char *requestBuffer);

#endif