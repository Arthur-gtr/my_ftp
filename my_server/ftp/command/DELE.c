/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** DELE
*/

#include "my_ftp.h"

#include <stdio.h>
#include <stdlib.h>

int dele(ftp_t *ftp, int index, char *command)
{   
    char path[DATA_BUFFER] = {0};

    if (is_connected(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    if (get_number_arg(command) > 2){
        dprintf(ftp->polling.fds[index].fd, "ftp 501 server cannot accept argument\r\n");
        return EXIT_SUCCESS;
    }
    get_n_arg(command, path, 2);
    if (*path == '/')
        prepend(path, ftp->server.serv_wd);
    if (is_file(path) == false){
        dprintf(ftp->polling.fds[index].fd, "550 Must be a file.\r\n");
        return EXIT_SUCCESS;
    }
    if (remove(path) == -1)
        return EXIT_FAILURE;
    dprintf(ftp->polling.fds[index].fd, "250 File deleted.\r\n");
    return EXIT_SUCCESS;
}