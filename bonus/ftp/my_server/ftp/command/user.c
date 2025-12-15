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
    for (; *cmp != '\0'; cmp++){
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
    return skip_case;
}

int hanlde_name(ftp_t *ftp, int index, char *command)
{
    if (strncmp(command, "Anonymous", 9) == 0){
        ftp->client_tab.client[CLIENT_IDX(index)].connection |= USER_C;
        dprintf(ftp->polling.fds[index].fd, "331 User name okay, need password.\r\n");
        return EXIT_SUCCESS;
    }
    if (strncmp(command, "GuiAdmin", 8) == 0){
        ftp->client_tab.client[CLIENT_IDX(index)].connection |= USER_GUI;
        ftp->client_tab.client[CLIENT_IDX(index)].connection |= USER_C;
        dprintf(ftp->polling.fds[index].fd, "331 Gui ok, pls set PASS for data acess\r\n");
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int user(ftp_t *ftp, int index, char *command)
{
    if (get_number_arg(command) > 2){
        dprintf(ftp->polling.fds[index].fd, ARG_501);
        return EXIT_SUCCESS;
    }
    ftp->client_tab.client[CLIENT_IDX(index)].connection = RESET_FLAG;
    command += skip_garbage("\t ", command, command_tab[USER].size);
    if (strlen(command) > LIMIT_NAME){
        ftp->client_tab.client[CLIENT_IDX(index)].connection = 0;
        dprintf(ftp->polling.fds[index].fd, "501 Username too long\r\n");
        return EXIT_SUCCESS;
    }
    if (hanlde_name(ftp, index, command) == EXIT_SUCCESS)
        return EXIT_SUCCESS;
    dprintf(ftp->polling.fds[index].fd, "530 Uknowing name...\r\n");
    return EXIT_SUCCESS;
}
