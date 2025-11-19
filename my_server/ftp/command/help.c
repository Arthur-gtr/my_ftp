/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** HELP
*/
#include "my_ftp.h"
#include "help.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

const char *help_tab[] = {
    HELP_USER,
    HELP_PASS,
    HELP_CWD,
    HELP_QUIT,
    HELP_DELE,
    HELP_PWD,
    HELP_PASV,
    HELP_PORT,
    HELP_HELP,
    HELP_NOOP,
    HELP_RETR,
    HELP_STOR,
    HELP_LIST,
    NULL
};

void dshow_tab(const char **tab, int fd)
{
    for (int i = 0; tab[i] != NULL; i++)
        write(fd, tab[i], strlen(tab[i]));
}

void show_cmd(char *cmd, int fd)
{
    int size_cmd = strlen(cmd);

    for (int i = 0; help_tab[i] != NULL; i++)
        if (strncmp(cmd, help_tab[i], size_cmd) == 0)
            dprintf(fd, "%s", help_tab[i]);
}

int help(ftp_t *ftp, int index, char *command)
{
    int nb_arg = get_number_arg(command);
    char cmd[5] = {0};

    if (is_connected(&ftp->client[CLIENT_IDX(index)],
        ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    if (nb_arg > 2){
        dprintf(ftp->polling.fds[index].fd, ARG_501);
        return EXIT_SUCCESS;
    }
    if (nb_arg == 1)
        dshow_tab(help_tab, ftp->polling.fds[index].fd);
    if (nb_arg == 2){
        get_n_arg(command, cmd, 2);
        show_cmd(cmd, ftp->polling.fds[index].fd);
    }
    return 0;
}
