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


int init_server(server_t *serv, int port, char *path)
{
    serv->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv->server_fd < 0)
        return reterr("Socket build error");
    serv->addr.sin_family = AF_INET;
    serv->addr.sin_addr.s_addr = INADDR_ANY;
    serv->addr.sin_port = htons(port);
    if (bind(serv->server_fd, (struct sockaddr *)&serv->addr,
        sizeof(serv->addr)) < 0) {
        close(serv->server_fd);
        return reterr("Bind Error");
    }
    if (listen(serv->server_fd, MAX_SIMULTANEOUS_CONNEXION) < 0) {
        close(serv->server_fd);
        return reterr("Listen Error");
    }
    if (realpath(path, serv->serv_wd) == NULL)
        return reterr("Work directory error");
    chdir(serv->serv_wd);
    serv->size_wd = strlen(serv->serv_wd);
    return EXIT_SUCCESS;
}
