/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** quit
*/

#include "command.h"
#include "my_ftp.h"

#include <stdlib.h>
#include <unistd.h>

int quit(ftp_t *ftp, int index, char *command)
{
    if (!(ftp->client[index - 1].connection & CONNECTED))
        return EXIT_FAILURE;
    if (get_number_arg(command) > 1){
        dprintf(ftp->polling.fds[index].fd, ARG_501);
        return EXIT_SUCCESS;
    }
    write(ftp->polling.fds[index].fd, "221\r\n", 5);
    ftp->polling.fds[index].fd = -1;
    ftp->polling.fds[index].revents = 0;
    return EXIT_FAILURE;
}
