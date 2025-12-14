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
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    client->datatransfer_mode = RESET_FLAG;
    if (socket_fd < 0)
        return EXIT_FAILURE;
    if (connect(socket_fd,
        (struct sockaddr *)&client->addr_port,
        sizeof(client->addr_port)) < 0) {
        close(socket_fd);
        dprintf(fd, "425 Can't open data connection");
    }
    client->socket_fd = socket_fd;
    return 0;
}

static
int accept_in_co(client_t *client, int fd)
{
    int pasv_fd = client->pasv_fd;
    int socket_fd = accept(client->pasv_fd,
        (struct sockaddr *)&client->addr,
        &client->addrlen);

    client->pasv_fd = -1;
    close(pasv_fd);
    if (socket_fd == -1){
        dprintf(fd, "425 Can't open data connection");
        close(socket_fd);
        return MALLOC_FAILED;
    }
    client->datatransfer_mode = RESET_FLAG;
    client->socket_fd = socket_fd;
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
