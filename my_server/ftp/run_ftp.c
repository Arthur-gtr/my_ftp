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


int get_data(ftp_t *ftp, int index)
{
    int status = 0;
    char buffer[DATA_BUFFER];
    /*ftp->client[CLIENT_IDX(index)].cmd_info.buffer*/

    status = read(ftp->polling.fds[index].fd, buffer, DATA_BUFFER);
    if (status == -1)
        return EXIT_FAILURE;
    if (status == 0){
        printf("Socket Closed: %d\n", index);
        close(ftp->polling.fds[index].fd);
        ftp->polling.fds[index].fd = -1;
        return EXIT_FAILURE;
    }
    buffer[status] = '\0';
    if (status + strlen(ftp->client[CLIENT_IDX(index)].cmd_info.buffer) > 2048){
        memset(ftp->client[CLIENT_IDX(index)].cmd_info.buffer, 0, DATA_BUFFER);
        strncpy(buffer, ftp->client[CLIENT_IDX(index)].cmd_info.buffer, DATA_BUFFER);
        dprintf(2, "Warning too much charcter stor in the command buffer\n");
        dprintf(ftp->polling.fds[index].fd, "500 Warning too much charcter stor in the command buffer next time you will be kick\r\n");
        return EXIT_FAILURE;
    }
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
int check_force_deco(struct pollfd *poll_fd)
{
    if (poll_fd->revents & (POLLHUP | POLLERR | POLLNVAL)){
            poll_fd->fd = -1;
            poll_fd->revents = 0;
            poll_fd->events = 0;
            printf("Deconect: force\n");
            return EXIT_FAILURE;
        }
    return EXIT_SUCCESS;
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
    cmd_info->nb_crlf = find_pattern_in_str(cmd_info->buffer, CRLF);

    if (cmd_info->nb_crlf > 0){
        printf("Number of CRLF in the command: %d\n", cmd_info->nb_crlf);
        return true;
    }
    return false;
}

int is_valid(char c)
{
    return isalpha(c) || isdigit(c) || ispunct(c);
}

static
int fill_current_cmd(ftp_command_t *cmd_info)
{
    int pos = 0;
    bool need_space = false;

    for(int i = cmd_info->pos; cmd_info->buffer[i] != '\0'; i++){
        if (strncmp(&cmd_info->buffer[i], CRLF, CRLF_SZ) == 0){
            cmd_info->pos = (i + CRLF_SZ);
            cmd_info->command[i] = '\0';
            cmd_info->garbage_status = (cmd_info->buffer[cmd_info->pos + 1] == '\0') ? false : true;
            strncat(cmd_info->command, CRLF, CMD_BUFFER - 1);
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

void print_visible(const char *s)
{
    for (int i = 0; s[i]; i++) {
        unsigned char c = s[i];

        switch (c) {
            case '\n': printf("\\n"); break;
            case '\r': printf("\\r"); break;
            case '\t': printf("\\t"); break;
            case '\v': printf("\\v"); break;
            case '\f': printf("\\f"); break;
            case '\a': printf("\\a"); break;
            case '\\': printf("\\\\"); break;
            default:
                if (c < 32 || c > 126) {
                    printf("\\x%02X", c);
                } else {
                    putchar(c);
                }
                break;
        }
    }
    printf("\n");
}

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
    printf("Position String: %d : %c\n",cmd_info->pos,  cmd_info->buffer[cmd_info->pos]);
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
    for (int i = 0; ftp->client[CLIENT_IDX(client)].cmd_info.nb_crlf != i; i++){
        if (fill_current_cmd(&ftp->client[CLIENT_IDX(client)].cmd_info) == EXIT_FAILURE)
            break;
        printf("ID: %d, Command: %s\n", i, ftp->client[CLIENT_IDX(client)].cmd_info.command);
        write(1, "Show CRLF:", 10);
        print_visible(ftp->client[CLIENT_IDX(client)].cmd_info.command);
        if (command_parsing(ftp, client) == MALLOC_FAILED){
            printf("Command Parsing failed\n");
            return MALLOC_FAILED;
        }
        memset(ftp->client[CLIENT_IDX(client)].cmd_info.command, 0, CMD_BUFFER);
    }
    if (ftp->client[CLIENT_IDX(client)].cmd_info.garbage_status == true){
        printf("Fill garbage\n");
        fill_garbage(&ftp->client[CLIENT_IDX(client)].cmd_info);
        printf("Garbage: %s\n", ftp->client[CLIENT_IDX(client)].cmd_info.garbage);
        ftp->client[CLIENT_IDX(client)].cmd_info.garbage_status = false;
    }
    return true;
}

static
void reset_cmd(ftp_command_t *cmd_info)
{
    memset(cmd_info->buffer, 0, DATA_BUFFER);
    printf("Put in buffer:\n");
    print_visible(cmd_info->buffer);
    print_visible(cmd_info->garbage);
    strncpy(cmd_info->buffer, cmd_info->garbage, CMD_BUFFER);
    memset(cmd_info->garbage, 0, DATA_BUFFER);
    memset(cmd_info->command, 0, CMD_BUFFER);
    printf("Reset garbage and command:\n");
    print_visible(cmd_info->buffer);
    print_visible(cmd_info->garbage);
    cmd_info->nb_arg = 0;
    cmd_info->nb_crlf = 0;
    cmd_info->pos = 0;
    cmd_info->size_cmd = 0;
}



static
int check_client_event(ftp_t *ftp, int i)
{
    int test = 0; 
    if (!(ftp->polling.fds[i].revents & POLLIN))
        return EXIT_SUCCESS;
    if (check_force_deco(&ftp->polling.fds[i]) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    if (get_data(ftp, i) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    if (command_detected(&ftp->client[CLIENT_IDX(i)].cmd_info) == true){
        //printf("Buffer=%s\n", ftp->client[CLIENT_IDX(i)].cmd_info.buffer);
        if(test = execute(ftp, i) == MALLOC_FAILED){
            printf("Execute failed\n");
            return MALLOC_FAILED;
        }
        printf("Not failed cause %d\n", test);
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
        if (check_client_event(ftp, i) == MALLOC_FAILED){
            printf("Check client Failed\n");
            return EXIT_FAILURE;
        }
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
