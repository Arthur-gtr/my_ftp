/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** run_ftp
*/

#include <poll.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <ctype.h>

#include "my_ftp.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>


static
void init_ftp_command(ftp_command_t *cmd_info)
{
    cmd_info->nb_arg = 0;
    cmd_info->nb_crlf = 0;
    cmd_info->size_cmd = 0;
    cmd_info->pos = 0;
    cmd_info->garbage_status = false;
    memset(cmd_info->buffer, 0, DATA_BUFFER);
    memset(cmd_info->command, 0, CMD_BUFFER);
    memset(cmd_info->garbage, 0, DATA_BUFFER);
}

int accept_connection(struct pollfd *poll_fd, int server_fd, client_t *client)
{
    poll_fd->fd = accept(server_fd, (struct sockaddr *)&client->addr, &client->addrlen);
    if (poll_fd->fd == -1)
        return reterr("Error file descriptor");
    poll_fd->events = POLLIN | POLLOUT;
    poll_fd->revents = 0;
    getsockname(poll_fd->fd, (struct sockaddr *)&client->addr, &client->addrlen);
    return 0;
}

int check_pollfd_size(polling_t *polling)
{
    struct pollfd *fds = NULL;

    if (polling->nfds > (int)polling->alloc_pollfd){
        polling->alloc_pollfd *= 2;
        fds = realloc(polling->fds, sizeof(struct pollfd) * polling->alloc_pollfd);
        if (fds == NULL) {
            free(polling->fds);
            return EXIT_FAILURE;
        }
        polling->fds = fds;
    }
    return EXIT_SUCCESS;
}

int check_client_size(client_t **client_tab)
{
    client_t *client = *client_tab;
    client_t *new_tab = NULL;

    if (client->size >= client->alloc_client){
        client->alloc_client *= 2;
        new_tab = realloc(client, sizeof(client_t) * client->alloc_client);
        if (new_tab == NULL) {
            free(client);
            return EXIT_FAILURE;
        }
        *client_tab = new_tab;
    }
    return EXIT_SUCCESS;
}

int add_user(ftp_t *ftp)
{   
    ftp->client->size++;
    ftp->polling.nfds++;
    if (check_pollfd_size(&ftp->polling) == EXIT_FAILURE || check_client_size(&ftp->client) == EXIT_FAILURE)
        return EXIT_FAILURE;
    ftp->client[ftp->client->size - 1].addrlen = sizeof(struct sockaddr_in);
    ftp->client[ftp->client->size - 1].connection = RESET_FLAG;
    ftp->client[ftp->client->size - 1].wd = strdup("/");
    if (ftp->client[ftp->client->size - 1].wd == NULL)
        reterr("Malloc failed");
    if (accept_connection(&ftp->polling.fds[ftp->client->size], ftp->server.server_fd, &ftp->client[ftp->client->size - 1]) == EXIT_FAILURE)
        return EXIT_FAILURE;
    write(ftp->polling.fds[ftp->client->size].fd, "220 Connection:\r\n", 17);
    printf("User add to the queu, IP: %s\n", inet_ntoa(ftp->client[ftp->client->size - 1].addr.sin_addr));
    init_ftp_command(&ftp->client[CLIENT_IDX(ftp->client->size)].cmd_info);
    return EXIT_SUCCESS;
}