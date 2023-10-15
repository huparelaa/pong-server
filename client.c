#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include "globals.h"

char *colors[NCOLORS] = {YELLOW, BLUE, MAGENTA,
                         CYAN, LYELLOW, LBLUE, LMAGENTA,
                         LCYAN, RED, GREEN, LRED, LGREEN};

/* function returns true if the two passes internet address are identical */
int clientCompare(struct sockaddr_in client1, struct sockaddr_in client2)
{
    if (strncmp((char *)&client1.sin_addr.s_addr, (char *)&client2.sin_addr.s_addr,
                sizeof(unsigned long)) == 0)
    {
        if (strncmp((char *)&client1.sin_port, (char *)&client2.sin_port, sizeof(unsigned short)) == 0)
        {
            if (strncmp((char *)&client1.sin_family, (char *)&client2.sin_family,
                        sizeof(unsigned short)) == 0)
            {

                return TRUE;
            }
        }
    }
    return FALSE;
}

void userColor(int n, char *response_buffer)
{
    strcat(response_buffer, colors[(n % NCOLORS)]);
}