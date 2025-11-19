/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** handle_co
*/
#include "my_ftp.h"
#include "command.h"

#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static
int accept_out_co(client_t *client, int fd)
{
    client->datatransfer_mode = RESET_FLAG;
    client->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->socket_fd < 0)
        return EXIT_FAILURE;
    if (connect(client->socket_fd,
        (struct sockaddr *)&client->addr_port,
        sizeof(client->addr_port)) < 0) {
        dprintf(fd, "425 Can't open data connection");
    }
    return 0;
}

static
int accept_in_co(client_t *client, int fd)
{
    client->datatransfer_mode = RESET_FLAG;
    client->socket_fd = accept(client->pasv_fd,
        (struct sockaddr *)&client->addr,
        &client->addrlen);
    if (client->socket_fd == -1){
        dprintf(fd, "425 Can't open data connection");
        return MALLOC_FAILED;
    }
    return EXIT_SUCCESS;
}

int accept_co(client_t *client, int fd)
{
    if (client->datatransfer_ready == false){
        dprintf(fd, "425 Can't open data connection.\r\n");
        return EXIT_FAILURE;
    }
    client->datatransfer_ready = false;
    if (client->datatransfer_mode == PASV)
        return accept_in_co(client, fd);
    if (client->datatransfer_mode == PORT)
        return accept_out_co(client, fd);
    printf("PASV NOT EXECUTE: %d\n", client->datatransfer_mode);
    return DATA_NOT_READY;
}
