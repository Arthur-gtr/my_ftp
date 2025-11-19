/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** pwd
*/
#include "my_ftp.h"

#include <stdlib.h>
#include <stdio.h>

bool is_connected(client_t *client, int fd)
{
    if (!(client->connection & CONNECTED)){
        dprintf(fd, "530 Please login with USER and PASS.\r\n");
        return false;
    }
    return true;
}

int pwd(ftp_t *ftp, int index, char *command)
{
    if (is_connected(&ftp->client[CLIENT_IDX(index)],
        ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    if (get_number_arg(command) > 1){
        dprintf(ftp->polling.fds[index].fd, ARG_501);
        return EXIT_SUCCESS;
    }
    dprintf(ftp->polling.fds[index].fd,
        "257 \"%s\"\r\n", ftp->client[CLIENT_IDX(index)].wd);
    return EXIT_SUCCESS;
}
