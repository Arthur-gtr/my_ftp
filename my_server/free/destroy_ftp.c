/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** init_ftp
*/

/*Network*/
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_ftp.h"

void destroy_server(server_t *server)
{
    close(server->server_fd);
}

void destroy_polling(polling_t *polling)
{
    printf("nfds; %d\n", polling->nfds);
    for (int i = CLIENT_ID_MIN; i != polling->nfds; i++){
        printf("Free nelement:%d\n", i);
        if (polling->fds[i].fd != -1){
            printf("Poll fds[i]: %d\n", polling->fds[i].fd);
            close(polling->fds[i].fd);
        }
    }
    free(polling->fds);
}

void destroy_client(client_t *client)
{
    free(client);
}

void destroy_ftp(ftp_t *ftp)
{
    destroy_server(&ftp->server);
    destroy_polling(&ftp->polling);
    destroy_client(ftp->client);
}
