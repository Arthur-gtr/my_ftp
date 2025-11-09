/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** init_polling
*/

/*Network*/
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 

#include <stdlib.h>

#include "my_ftp.h"

int init_polling(polling_t *polling, int fd_server)
{
    polling->fds = malloc(sizeof(struct pollfd) * 1);
    if (polling->fds == NULL)
        return reterr("Malloc failed");
    polling->nfds = 1;
    polling->alloc_pollfd = 1;
    polling->fds[FDS_SERVER].fd = fd_server;
    polling->fds[FDS_SERVER].events = POLLIN;
    return EXIT_SUCCESS;
}