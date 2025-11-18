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

bool char_in_str(char c, char *cmp)
{
    for (; *cmp != '\0' ; cmp++){
        if (*cmp == c)
            continue;
        return false;
    }
    return true;
}

int skip_garbage(char *garbage, char *command, int init_pos)
{
    int skip_case = init_pos + 1;

    for (; char_in_str(command[skip_case], garbage); skip_case++);
    printf("size; %d\n", skip_case);
    return skip_case;
}

int user(ftp_t *ftp, int index, char *command)
{
    if (get_number_arg(command) > 2){
        dprintf(ftp->polling.fds[index].fd, "ftp 501 server cannot accept argument\r\n");
        return EXIT_SUCCESS;
    }
    ftp->client[CLIENT_IDX(index)].connection = RESET_FLAG;
    command += skip_garbage("\t ", command, command_tab[USER].size);
    if (strlen(command) > LIMIT_NAME){
        ftp->client[CLIENT_IDX(index)].connection = 0;
        write(ftp->polling.fds[index].fd, "501 Username too long\r\n", 23);
        return EXIT_FAILURE;
    }
    if (strncmp(command, "Anonymous", 9) == 0){
        ftp->client[CLIENT_IDX(index)].connection |= USER_C;
        write(ftp->polling.fds[index].fd, "331 Username Accepted\r\n", 23);
        return EXIT_SUCCESS;
    }
    printf("Failed USER\n");
    write(ftp->polling.fds[index].fd, "530 Uknowing name...\r\n", 23);
    return EXIT_FAILURE;
}