/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** init_ftp
*/

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdlib.h>

#include "my_ftp.h"

#include "init.h"

int init_ftp(ftp_t *ftp, char *port, char *path)
{
    ftp->running = RUNNING;
    if (init_server(&ftp->server, atoi(port), path) == EXIT_FAILURE)
        return EXIT_FAILURE;
    ftp->client = malloc(sizeof(client_t));
    if (ftp->client == NULL)
        return EXIT_FAILURE;
    init_client(ftp->client);
    if (init_polling(&ftp->polling, ftp->server.server_fd) == EXIT_FAILURE)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
