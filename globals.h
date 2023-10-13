#include <string.h>

// ROOMS
#define MAX_CLIENTS 2
#define MAX_ROOMS 10
#define USERNAME_LEN 32
#define MAX_MESSAGE_LENGTH 256
#define BUF_SIZE 1024

//GLOBAL VARIABLES
#define TRUE 1
#define FALSE 0
#define SYSERR -1
#define OK 0

//SERVER
#define PORT 8888
#define IP "127.0.0.1"

//ACTIONS
#define ERROR "error\n"
#define CLOSE "close\n"
#define EXIT "exit\n"
#define USERNAMExMESSAGE ": "
#define ROOM "/room"
#define SHOW_CLIENTS "/show\n"

// Colors
#define NCOLORS 10
#define RED   "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE   "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN   "\x1B[36m"
#define LRED   "\x1B[91m"
#define LGREEN   "\x1B[92m"
#define LYELLOW   "\x1B[93m"
#define LBLUE   "\x1B[94m"
#define LMAGENTA  "\x1B[95m"
#define LCYAN   "\x1B[96m"
#define WHITE  "\x1B[37m"
#define RESET "\x1B[0m"