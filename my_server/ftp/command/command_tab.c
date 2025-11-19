/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** command_tab
*/

#include "command.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



int len_array(char **array)
{
    int i = 0;

    for (; array[i] != NULL; i++);
    return i;
}

int test(ftp_t *ftp, int index, char *command)
{
    write(ftp->polling.fds[index].fd,
        "502 command foud but not implemented\r\n", 38);
    return EXIT_SUCCESS;
}

int quit(ftp_t *ftp, int index, char *command)
{
    write(ftp->polling.fds[index].fd, "221 quit\r\n", 10);
    ftp->polling.fds[index].fd = -1;
    return EXIT_SUCCESS;
}

int command_parsing(ftp_t *ftp, int index)
{
    int status = -2;

    printf("Command::%s\n", ftp->client[CLIENT_IDX(index)].cmd_info.command);
    for (size_t i = 0; i != sz_tab_cmd; i++){
        if (strncmp(command_tab[i].command_name, ftp->client[CLIENT_IDX(index)].cmd_info.command, command_tab[i].size) == 0){
            status = command_tab[i].funct(ftp, index, ftp->client[CLIENT_IDX(index)].cmd_info.command);
        }
        if (status == MALLOC_FAILED || status == EXIT_SUCCESS){
            return status;
        }
    }
    write(ftp->polling.fds[index].fd,
        "500 Syntax error, command unrecognized.\r\n", 41);
    return reterr("500 Syntax error, command unrecognized.");
}
