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

int test(ftp_t *ftp, int index, __attribute_maybe_unused__ char *command)
{
    dprintf(ftp->polling.fds[index].fd,
        "502 command foud but not implemented\r\n");
    return EXIT_SUCCESS;
}

int command_parsing(ftp_t *ftp, int index)
{
    int status = -2;

    printf("Command::%s", ftp->client_tab.client[CLIENT_IDX(index)].cmd_info.command);
    for (size_t i = 0; i != sz_tab_cmd; i++){
        if (strncmp(command_tab[i].command_name,
            ftp->client_tab.client[CLIENT_IDX(index)].cmd_info.command,
            command_tab[i].size) == 0){
            status = command_tab[i].funct(ftp,
                index,
                ftp->client_tab.client[CLIENT_IDX(index)].cmd_info.command
            );
        }
        if (status == MALLOC_FAILED || status == EXIT_SUCCESS){
            return status;
        }
    }
    dprintf(ftp->polling.fds[index].fd,
        "500 Syntax error, command unrecognized.\r\n");
    return reterr("500 Syntax error, command unrecognized.");
}
