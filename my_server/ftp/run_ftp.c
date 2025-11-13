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
    if (status + strlen(ftp->client[CLIENT_IDX(index)].cmd_info.buffer) > 2048)
        dprintf(2, "Warning too much charcter stor in the command buffer\n");
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
int find_pattern_in_str(char *str, char *pattern)
{
    int count = 0;
    int pattern_sz = strlen(pattern);

    for (int i = 0; str[i] != '\0'; i++) {
        if (strncmp(&str[i], pattern, pattern_sz) == 0)
            count++;
    }
    return count;
}

static
bool command_detected(ftp_command_t *cmd_info)
{
    cmd_info->nb_crlf = find_pattern_in_str(cmd_info->buffer, "\r\n");

    if (cmd_info->nb_crlf > 0){
        printf("CRLF: %d\n", cmd_info->nb_crlf);
        return true;
    }
    return false;
}

int is_valid(char c)
{
    return isalpha(c)
        || isdigit(c)
        || ispunct(c);
}

static
int fill_current_cmd(ftp_command_t *cmd_info)
{
    int pos = 0;
    bool need_space = (is_valid(cmd_info->buffer[0])) ? true : false;

    for(int i = cmd_info->pos; cmd_info->buffer[i] != '\0'; i++){
        if (strncmp(&cmd_info->buffer[i], CRLF, CRLF_SZ) == 0){
            cmd_info->pos = i;
            cmd_info->command[pos] = '\0';
            return EXIT_SUCCESS;
        }
        if (is_valid(cmd_info->buffer[i]) ){
            cmd_info->command[pos] = cmd_info->buffer[i];
            pos++;
            need_space = true;
            continue;
        }
        if ((cmd_info->buffer[i] == ' ' || cmd_info->buffer[i] == '\t') && need_space == true){
            cmd_info->command[pos] = ' ';
            pos++;
            need_space = false;
            continue;
        }
    }
    return EXIT_FAILURE;
}

static
bool execute(ftp_t *ftp, int client)
{
    for (int i = 0; ftp->client[CLIENT_IDX(client)].cmd_info.nb_crlf != i; i++){
        if (fill_current_cmd(&ftp->client[CLIENT_IDX(client)].cmd_info) == EXIT_FAILURE){
            /*On balance tous dans le buffer garbage qui est fais pour ça*/
            printf("Garbage\n");
        }
        printf("Execution %d\n", i);
        printf("Restore Command: %s\n", ftp->client[CLIENT_IDX(client)].cmd_info.command);
    }
    return true;
}

static
int check_client_event(ftp_t *ftp, int i)
{
    if (!(ftp->polling.fds[i].revents & POLLIN))
        return EXIT_SUCCESS;
    check_force_deco(&ftp->polling.fds[i]);
    if (get_data(ftp, i) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    if (command_detected(&ftp->client[CLIENT_IDX(i)].cmd_info) == true){
        printf("Buffer=%s\n", ftp->client[CLIENT_IDX(i)].cmd_info.buffer);
        execute(ftp, i);
        printf("Command=%s\n", ftp->client[CLIENT_IDX(i)].cmd_info.command);
    }
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
