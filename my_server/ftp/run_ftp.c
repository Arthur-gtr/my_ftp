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
    cmd_info->nb_crlf = find_pattern_in_str(cmd_info->buffer, CRLF);
    if (cmd_info->nb_crlf > 0)
        return true;
    return false;
}

static
void strnlastcat(char *dest, const char *src, size_t n)
{
    size_t len = strlen(src);
    int pos = 0;

    if (len < n)
        n = len;
    for (size_t i = n; src[i] != '\0'; i++) {
        dest[pos] = src[i];
        pos++;
    }
}

static
void fill_garbage(ftp_command_t *cmd_info)
{
    int garbage_size = (strlen(cmd_info->buffer) - cmd_info->pos);

    if (garbage_size + strlen(cmd_info->garbage) > DATA_BUFFER){
        memset(cmd_info->garbage, 0, DATA_BUFFER);
        strnlastcat(cmd_info->garbage, cmd_info->buffer, cmd_info->pos);
        dprintf(2, "Warning too much charcter stor in the command buffer\n");
    }
    strnlastcat(cmd_info->garbage, cmd_info->buffer, cmd_info->pos);
}

static
int execute(ftp_t *ftp, int client)
{
    for (int i = 0; ftp->client[CLIENT_IDX(client)].cmd_info.nb_crlf != i;
        i++){
        if (fill_current_cmd(&ftp->client[CLIENT_IDX(client)].cmd_info)
            == EXIT_FAILURE)
            break;
        if (command_parsing(ftp, client) == MALLOC_FAILED)
            return MALLOC_FAILED;
        memset(ftp->client[CLIENT_IDX(client)].cmd_info.command,
            0, CMD_BUFFER);
    }
    if (ftp->client[CLIENT_IDX(client)].cmd_info.garbage_status == true){
        fill_garbage(&ftp->client[CLIENT_IDX(client)].cmd_info);
        ftp->client[CLIENT_IDX(client)].cmd_info.garbage_status = false;
    }
    return true;
}

static
void reset_cmd(ftp_command_t *cmd_info)
{
    memset(cmd_info->buffer, 0, DATA_BUFFER);
    strncpy(cmd_info->buffer, cmd_info->garbage, CMD_BUFFER);
    memset(cmd_info->garbage, 0, DATA_BUFFER);
    memset(cmd_info->command, 0, CMD_BUFFER);
    cmd_info->nb_arg = 0;
    cmd_info->nb_crlf = 0;
    cmd_info->pos = 0;
    cmd_info->size_cmd = 0;
}

static
int check_client_event(ftp_t *ftp, int i)
{
    if (ftp->polling.fds[i].fd == -1)
        return EXIT_SUCCESS;
    if (!(ftp->polling.fds[i].revents & POLLIN))
        return EXIT_SUCCESS;
    if (check_force_deco(&ftp->polling.fds[i]) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    if (get_data(ftp, i) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    if (command_detected(&ftp->client[CLIENT_IDX(i)].cmd_info) == true){
        if (execute(ftp, i) == MALLOC_FAILED)
            return MALLOC_FAILED;
        reset_cmd(&ftp->client[CLIENT_IDX(i)].cmd_info);
    }
    return EXIT_SUCCESS;
}

static
int check_event(ftp_t *ftp)
{
    if (check_server_event(ftp) == EXIT_FAILURE)
        return EXIT_FAILURE;
    for (int i = CLIENT_ID_MIN; i < ftp->polling.nfds; i++)
        if (check_client_event(ftp, i) == MALLOC_FAILED)
            return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

#include <signal.h>
#include <errno.h>

volatile sig_atomic_t running = RUNNING;

void stop_ftp(int sig)
{
    (void)sig;
    running = 1;
}


int run_ftp(ftp_t *ftp)
{
    int status;
    
    signal(SIGINT, stop_ftp);
    while (running == RUNNING){
        status = poll(ftp->polling.fds, ftp->polling.nfds, TIMEOUT);
        if (errno == EINTR)
            continue;
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
