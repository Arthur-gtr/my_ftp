/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** USER
*/

#include "command.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int user(ftp_t *ftp, int index, char *command)
{
    ftp->client[index].connection = RESET_FLAG;
    command += strlen(command_tab[0].command_name) + 1;
    if (strlen(command) > LIMIT_NAME){
        ftp->client[index].connection = 0;
        write(ftp->polling.fds[index].fd, "501 Username too long\r\n", 23);
        return EXIT_FAILURE;
    }
    if (strcmp(command, "anonymous\r\n") == 0){
        ftp->client[index].connection |= USER_C;
        write(ftp->polling.fds[index].fd, "331 Username Accepted\r\n", 23);
        return EXIT_SUCCESS;
    }
    write(ftp->polling.fds[index].fd, "530 Uknowing name...\r\n", 23);
    return EXIT_FAILURE;
}