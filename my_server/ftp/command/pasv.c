/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** pasv
*/

#include "my_ftp.h"
#include <netinet/in.h>
#include "command.h"
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int init_pasvdadrr(struct sockaddr_in *addr_pasv, int pasv_fd)
{
    addr_pasv->sin_family = AF_INET;
    addr_pasv->sin_addr.s_addr = INADDR_ANY;
    addr_pasv->sin_port = htons(0);
    if (bind(pasv_fd, (struct sockaddr *)addr_pasv, sizeof(*addr_pasv)) < 0) {
        close(pasv_fd);
        return reterr("Bind Error");
    }
    if (listen(pasv_fd, MAX_SIMULTANEOUS_CONNEXION) < 0){
        close(pasv_fd);
        return reterr("Listen Error");
    }
    return 0;
}

int init_pasv(client_t *client)
{
    client->pasv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->pasv_fd < 0)
        return reterr("Socket build error");
    if (init_pasvdadrr(&client->addr_pasv, client->pasv_fd) == EXIT_FAILURE)
        return EXIT_FAILURE;
    client->addrlen_pasv = sizeof(client->addr_pasv);
    if (getsockname(client->pasv_fd,
        (struct sockaddr *)&client->addr_pasv,
        &client->addrlen_pasv) < 0) {
        close(client->pasv_fd);
        return reterr("getsockname Error");
    }
    return EXIT_SUCCESS;
}

int check_pasvarg(ftp_t *ftp, int index, char *command)
{
    if (is_connected(&ftp->client[CLIENT_IDX(index)],
        ftp->polling.fds[index].fd) == false)
        return EXIT_FAILURE;
    if (get_number_arg(command) > 1){
        ftp->client[CLIENT_IDX(index)].datatransfer_ready = false;
        dprintf(ftp->polling.fds[index].fd, ARG_501);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int pasv(ftp_t *ftp, int index, char *command)
{
    unsigned char *ip = NULL;
    int port = 0;

    if (check_pasvarg(ftp, index, command) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    if (init_pasv(&ftp->client[CLIENT_IDX(index)]) == EXIT_FAILURE){
        ftp->client[CLIENT_IDX(index)].datatransfer_ready = false;
        return MALLOC_FAILED;
    }
    ftp->client[CLIENT_IDX(index)].datatransfer_ready = true;
    ftp->client[CLIENT_IDX(index)].datatransfer_mode = PASV;
    ip = (unsigned char *)&ftp->client[CLIENT_IDX(index)].addr.sin_addr;
    port = ntohs(ftp->client[CLIENT_IDX(index)].addr_pasv.sin_port);
    dprintf(ftp->polling.fds[index].fd,
        "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d).\r\n",
        ip[0], ip[1], ip[2], ip[3], port / 256, port % 256);
    return EXIT_SUCCESS;
}
