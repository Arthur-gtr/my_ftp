/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** fill_current_cmd
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
int check_crlf(char *buffer, int i, ftp_command_t *cmd_info)
{
    if (strncmp(buffer, CRLF, CRLF_SZ) == 0){
            cmd_info->pos = (i + CRLF_SZ);
            cmd_info->command[i] = '\0';
            cmd_info->garbage_status =
                (cmd_info->buffer[cmd_info->pos + 1] == '\0') ? false : true;
            strncat(cmd_info->command, CRLF, CMD_BUFFER - 1);
            return EXIT_SUCCESS;
        }
    return EXIT_FAILURE;
}

int fill_current_cmd(ftp_command_t *cmd_info)
{
    int pos = 0;
    bool need_space = false;

    for (int i = cmd_info->pos; cmd_info->buffer[i] != '\0'; i++){
        if (check_crlf(&cmd_info->buffer[i], i, cmd_info) == EXIT_SUCCESS)
            return EXIT_SUCCESS;
        if (is_valid(cmd_info->buffer[i])){
            cmd_info->command[pos] = cmd_info->buffer[i];
            pos++;
            need_space = true;
            continue;
        }
        if ((cmd_info->buffer[i] == ' ' || cmd_info->buffer[i] == '\t')
            && need_space == true){
            cmd_info->command[pos] = ' ';
            pos++;
            need_space = false;
        }
    }
    return EXIT_FAILURE;
}
