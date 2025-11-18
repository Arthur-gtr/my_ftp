/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** cmd_tab
*/
#include "command.h"
#include "my_ftp.h"

#include <stdio.h>

#define TEST &test

int syst(ftp_t *ftp, int index, char *command);
int feat(ftp_t *ftp, int index, char *command);
int user(ftp_t *ftp, int index, char *command);
int password(ftp_t *ftp, int index, char *command);
int test(ftp_t *ftp, int index, char *command);
int list(ftp_t *ftp, int index, char *command);


int type(ftp_t *ftp, int index, char *command){
    dprintf(ftp->polling.fds[index].fd, "200 Okay\r\n");
    return 0;
}

const command_t command_tab[] = {
    {.command_name = "CDUP", .size = 4, .data_transfer =  false, .nb_argument = 0, .funct = TEST},
    {.command_name = "CWD",  .size = 3, .data_transfer =  false, .nb_argument = 1, .funct = TEST},
    {.command_name = "DELE", .size = 4, .data_transfer =  false, .nb_argument = 1, .funct = TEST},
    {.command_name = "HELP", .size = 4, .data_transfer =  false, .nb_argument = 0, .funct = TEST},
    {.command_name = "LIST", .size = 4, .data_transfer =  false, .nb_argument = 0, .funct = &list},
    {.command_name = "NOOP", .size = 4, .data_transfer =  false, .nb_argument = 0, .funct = TEST},
    {.command_name = "PASS", .size = 4, .data_transfer =  false, .nb_argument = 1, .funct = &password},
    {.command_name = "PASV", .size = 4, .data_transfer =  false, .nb_argument = 0, .funct = &pasv},
    {.command_name = "PORT", .size = 4, .data_transfer =  false, .nb_argument = 1, .funct = TEST},
    {.command_name = "PWD",  .size = 3, .data_transfer =  false, .nb_argument = 0, .funct = &pwd},
    {.command_name = "QUIT", .size = 4, .data_transfer =  false, .nb_argument = 0, .funct = TEST/*&quit*/},
    {.command_name = "RETR", .size = 4, .data_transfer =  false, .nb_argument = 1, .funct = TEST},
    {.command_name = "STOR", .size = 4, .data_transfer =  false, .nb_argument = 1, .funct = TEST},
    {.command_name = "USER", .size = 4, .data_transfer =  false, .nb_argument = 1, .funct = &user},
    {.command_name = "TYPE", .size = 4, .data_transfer =  false, .nb_argument = 1, .funct = &type}
};

const size_t sz_tab_cmd = sizeof(command_tab) / sizeof(*command_tab);