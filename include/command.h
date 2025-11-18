/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** my
*/


#ifndef COMMAND_H
    #define COMMAND_H
    #include <stdint.h>
    #include <stdbool.h>
    #include "my_ftp.h"
    #define LIMIT_NAME 256
    #define CDUP 0
    #define CWD 1
    #define DELE 2
    #define HELP 3
    #define LIST 4
    #define NOOP 5
    #define PASS 6
    #define PASV 7
    #define PORT 8
    #define PWD 9
    #define QUIT 10
    #define RETR 11
    #define STOR 12
    #define USER 13

    typedef struct command_s {
        char *command_name;
        int size;
        /*Pour savoir si il faut fork*/
        bool data_transfer;
        int nb_argument;
        int (*funct)(ftp_t *ftp, int index, char *buffer);
    } command_t;


    extern const command_t command_tab[];
    extern const size_t sz_tab_cmd;
#endif /*COMMAND_H*/
