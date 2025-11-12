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

    #define LIMIT_NAME 256
    
    #include "my_ftp.h"

    typedef struct command_s {
        /*Pour savoir si il faut fork*/
        char *command_name;
        int size;
        bool data_transfer;
        int nb_argument;
        int (*funct)(ftp_t *ftp, int , char *buffer);
    } command_t;

    extern const command_t command_tab[];
    extern const size_t sz_tab_cmd;
#endif /*COMMAND_H*/