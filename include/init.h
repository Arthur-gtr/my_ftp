/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/


#ifndef INIT_H
    #define INIT_H

    #include "my_ftp.h"

int init_ftp(ftp_t *ftp, char *port, char *path);
int init_polling(polling_t *polling, int fd_server);
int init_server(server_t *serv, int port, char *path);
int init_client(client_t *client);

#endif /*INIT_H*/
