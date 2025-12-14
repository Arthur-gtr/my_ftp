/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** run_ftp
*/

#include <poll.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <ctype.h>

#include "my_ftp.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>


int check_server_event(ftp_t *ftp)
{
    if (ftp->polling.fds[FDS_SERVER].revents & POLLIN){
        ftp->polling.fds[FDS_SERVER].revents = RESET_FLAG;
        if (add_user(ftp) == EXIT_FAILURE)
            return reterr("Malloc failed.");
    }
    return EXIT_SUCCESS;
}

int check_force_deco(struct pollfd *poll_fd)
{
    if (poll_fd->revents & (POLLHUP | POLLERR | POLLNVAL)){
            close(poll_fd->fd);
            poll_fd->fd = -1;
            poll_fd->revents = 0;
            poll_fd->events = 0;
            printf("Deconect: force\n");
            return EXIT_FAILURE;
        }
    return EXIT_SUCCESS;
}
