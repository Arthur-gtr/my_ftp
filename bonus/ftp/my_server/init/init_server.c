/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** init_server
*/

/*Network*/
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <string.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "my_ftp.h"

int init_serv_path(server_t *serv, int server_fd, char *path)
{
    if (realpath(path, serv->serv_wd) == NULL){
        close(server_fd);
        return reterr("Work directory error");
    }
    chdir(serv->serv_wd);
    serv->size_wd = strlen(serv->serv_wd);
    return EXIT_SUCCESS;
}

int init_server(server_t *serv, int port, char *path)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
        return reterr("Socket build error");
    serv->addr.sin_family = AF_INET;
    serv->addr.sin_addr.s_addr = INADDR_ANY;
    serv->addr.sin_port = htons(port);
    if (bind(server_fd, (struct sockaddr *)&serv->addr,
        sizeof(serv->addr)) < 0) {
        close(server_fd);
        return reterr("Bind Error");
    }
    if (listen(server_fd, MAX_SIMULTANEOUS_CONNEXION) < 0) {
        close(server_fd);
        return reterr("Listen Error");
    }
    if (init_serv_path(serv, server_fd, path) == EXIT_FAILURE)
        return EXIT_FAILURE;
    serv->server_fd = server_fd;
    return EXIT_SUCCESS;
}
