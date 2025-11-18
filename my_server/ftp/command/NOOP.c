/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** NOOP
*/

#include "my_ftp.h"

#include <stdlib.h>
#include <stdio.h>

int noop(ftp_t *ftp, int index, char *command)
{
    if (is_connected(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    if (get_number_arg(command) > 1){
        dprintf(ftp->polling.fds[index].fd, "ftp 501 server cannot accept argument\r\n");
        return EXIT_SUCCESS;
    }
    dprintf(ftp->polling.fds[index].fd, "200 NOOP ok.\r\n");
    return EXIT_SUCCESS;
}