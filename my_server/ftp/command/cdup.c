/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** CDUP
*/

#include "my_ftp.h"

#include <stdlib.h>
#include <stdio.h>

int cdup(ftp_t *ftp, int index, char *command)
{
    if (is_connected(&ftp->client[CLIENT_IDX(index)],
        ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    if (get_number_arg(command) > 1){
        dprintf(ftp->polling.fds[index].fd,
            "ftp 501 server cannot accept argument\r\n");
        return EXIT_SUCCESS;
    }
    return cwd(ftp, index, CDUP_ARG);
}
