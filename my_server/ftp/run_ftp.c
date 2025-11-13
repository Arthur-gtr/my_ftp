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
    memset(cmd_info->buffer, 0, 2048);
}

static
int add_user(ftp_t *ftp)
{   
    client_t *client = NULL;
    struct pollfd *fds = NULL;

    ftp->client->size++;
    ftp->polling.nfds++;
    if (ftp->polling.nfds > (int)ftp->polling.alloc_pollfd){
        ftp->polling.alloc_pollfd *= 2;
        fds = realloc(ftp->polling.fds, sizeof(struct pollfd) * ftp->polling.alloc_pollfd);
        if (fds == NULL) {
            free(ftp->polling.fds);
            return EXIT_FAILURE;
        }
        ftp->polling.fds = fds;
    }
    if (ftp->client->size >= ftp->client->alloc_client){
        ftp->client->alloc_client *= 2;
        client = realloc(ftp->client, sizeof(client_t) * ftp->client->alloc_client);
        if (client == NULL) {
            free(ftp->client);
            return EXIT_FAILURE;
        }
        ftp->client = client;
    }
    ftp->client[ftp->client->size - 1].addrlen = sizeof(ftp->client[0].addr);
    ftp->client[ftp->client->size - 1].connection = 0;
    ftp->client[ftp->client->size - 1].wd = strdup("/");
    if (ftp->client[ftp->client->size - 1].wd == NULL)
        reterr("Malloc failed");
    ftp->polling.fds[ftp->client->size].fd = accept(ftp->server.server_fd, (struct sockaddr *)&ftp->client[ftp->client->size - 1].addr, &ftp->client[ftp->client->size - 1].addrlen);
    ftp->polling.fds[ftp->client->size].events = POLLIN;
    ftp->polling.fds[ftp->client->size].revents = 0;
    if (ftp->polling.fds[ftp->client->size].fd == -1)
        return reterr("Error file descriptor");
    write(ftp->polling.fds[ftp->client->size].fd, "220 Connection:\r\n", 17);
    printf("User add to the queu, IP: %s\n", inet_ntoa(ftp->client[ftp->client->size - 1].addr.sin_addr));
    init_ftp_command(&ftp->client[CLIENT_IDX(ftp->client->size)].cmd_info);
    return EXIT_SUCCESS;
}

int get_data(ftp_t *ftp, int index)
{
    int status = 0;
    char buffer[2048];
    /*ftp->client[CLIENT_IDX(index)].cmd_info.buffer*/

    status = read(ftp->polling.fds[index].fd, buffer, 2048);
    if (status == -1)
        return EXIT_FAILURE;
    if (status == 0){
        printf("Socket Closed: %d\n", index);
        close(ftp->polling.fds[index].fd);
        ftp->polling.fds[index].fd = -1;
        return EXIT_FAILURE;
    }
    buffer[status] = '\0';
    strcat(ftp->client[CLIENT_IDX(index)].cmd_info.buffer, buffer);
    return EXIT_SUCCESS;
}

static
int check_server_event(ftp_t *ftp)
{
    if (ftp->polling.fds[FDS_SERVER].revents & POLLIN)
        if (add_user(ftp) == EXIT_FAILURE)
            return reterr("Malloc failed.");
    return EXIT_SUCCESS;
}

static
void check_force_deco(struct pollfd *poll_fd)
{
    if (poll_fd->revents & (POLLHUP | POLLERR | POLLNVAL)){
            poll_fd->fd = -1;
            poll_fd->revents = 0;
            printf("Deconect: force\n");
        }
}

/*Return the number of patern find in a str*/
static
int find_patern_in_str(char *str, char *patern)
{
    int count = 0;

    for (int i = 0; str[i] != '\0'; i++){
        if (strncmp(str, patern, i) == 0)
            count++;
    }
    return count;
}

static
bool command_detected(ftp_command_t *cmd_info)
{
    cmd_info->nb_crlf = find_patern_in_str(cmd_info->buffer, "\r\n");

    if (cmd_info->nb_crlf > 0){
        return true;
    }
    return false;
}

static
bool execute(ftp_command_t *cmd_info)
{
}

static
int check_client_event(ftp_t *ftp, int i)
{
    if (!(ftp->polling.fds[i].revents & POLLIN))
        return EXIT_SUCCESS;
    check_force_deco(&ftp->polling.fds[i]);
    if (get_data(ftp, i) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    if (command_detected(&ftp->client[CLIENT_IDX(i)].cmd_info) == true)
        printf("Command=%s\n", ftp->client[CLIENT_IDX(i)].cmd_info.buffer);
        //command_parsing(ftp, i);
    return EXIT_SUCCESS;
}

static
int check_event(ftp_t *ftp)
{
    if (check_server_event(ftp) == EXIT_FAILURE)
        return EXIT_FAILURE;   
    for (int i = CLIENT_ID_MIN; i < ftp->polling.nfds; i++)
        check_client_event(ftp, i);
    return EXIT_SUCCESS;
}

int run_ftp(ftp_t *ftp)
{
    int status;

    while (ftp->running == RUNNING){
        status = poll(ftp->polling.fds, ftp->polling.nfds, TIMEOUT);
        if (status == 0){
            printf("Timeout\n");
            continue;
        }
        if (status < 0)
            return reterr("Poll Error");
        if (check_event(ftp) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
