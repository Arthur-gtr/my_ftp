/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** cmd_tab
*/
#include "command.h"
#define CACA &test

int syst(ftp_t *ftp, int index, char *command);
int feat(ftp_t *ftp, int index, char *command);
int user(ftp_t *ftp, int index, char *command);
int password(ftp_t *ftp, int index, char *command);
int test(ftp_t *ftp, int index, char *command);

const command_t command_tab[] = {
    {.command_name = "CDUP\r\n", .data_transfer =  false, .nb_argument = 0, .funct = CACA},
    {.command_name = "CWD\r\n",  .data_transfer =  false, .nb_argument = 1, .funct = CACA},
    {.command_name = "DELE\r\n", .data_transfer =  false, .nb_argument = 1, .funct = CACA},
    {.command_name = "HELP\r\n", .data_transfer =  false, .nb_argument = 0, .funct = CACA},
    {.command_name = "LIST\r\n", .data_transfer =  false, .nb_argument = 0, .funct = CACA},
    {.command_name = "NOOP\r\n", .data_transfer =  false, .nb_argument = 0, .funct = CACA},
    {.command_name = "PASS\r\n", .data_transfer =  false, .nb_argument = 1, .funct = &password},
    {.command_name = "PASV\r\n", .data_transfer =  false, .nb_argument = 0, .funct = CACA},
    {.command_name = "PORT\r\n", .data_transfer =  false, .nb_argument = 1, .funct = CACA},
    {.command_name = "PWD\r\n",  .data_transfer =  false, .nb_argument = 0, .funct = CACA},
    {.command_name = "QUIT\r\n", .data_transfer =  false, .nb_argument = 0, .funct = CACA/*&quit*/},
    {.command_name = "RETR\r\n", .data_transfer =  false, .nb_argument = 1, .funct = CACA},
    {.command_name = "STOR\r\n", .data_transfer =  false, .nb_argument = 1, .funct = CACA},
    {.command_name = "USER\r\n", .data_transfer =  false, .nb_argument = 1, .funct = &user}
};

const size_t sz_tab_cmd = sizeof(command_tab) / sizeof(*command_tab);