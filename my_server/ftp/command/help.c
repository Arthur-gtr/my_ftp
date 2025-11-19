/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** HELP
*/
#include "my_ftp.h"

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

const char *help_tab[] = {
    "USER <SP> <username> <CRLF>   : Specify user for authentication\r\n",
    "PASS <SP> <password> <CRLF>   : Specify password for authentication\r\n",
    "CWD  <SP> <pathname> <CRLF>   : Change working directory\r\n",
    "CDUP <CRLF>                   : Change working directory to parent directory\r\n",
    "QUIT <CRLF>                   : Disconnection\r\n",
    "DELE <SP> <pathname> <CRLF>   : Delete file on the server\r\n",
    "PWD  <CRLF>                   : Print working directory\r\n",
    "PASV <CRLF>                   : Enable \"passive\" mode for data transfer\r\n",
    "PORT <SP> <host-port> <CRLF>  : Enable \"active\" mode for data transfer\r\n",
    "HELP [<SP> <string>] <CRLF>   : List available commands\r\n",
    "NOOP <CRLF>                   : Do nothing\r\n",
    "RETR <SP> <pathname> <CRLF>   : Download file from server to client\r\n",
    "STOR <SP> <pathname> <CRLF>   : Upload file from client to server\r\n",
    "LIST [<SP> <pathname>] <CRLF> : List files in the current working directory\r\n",
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
            dprintf(fd,"%s", help_tab[i]);
}

int help(ftp_t *ftp, int index, char *command)
{
    int nb_arg = get_number_arg(command);
    char cmd[5] = {0};
    
    if (is_connected(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    if (nb_arg > 2){
        dprintf(ftp->polling.fds[index].fd, "ftp 501 server cannot accept argument\r\n");
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