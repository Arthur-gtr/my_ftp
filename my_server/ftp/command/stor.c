/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** RETR
*/

#include "my_ftp.h"
#include "command.h"
#include "utils.h"

#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int get_from_client(char path[PATH_MAX], int socket_fd)
{
    int status = 1;
    int fd = open(path, O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
    char buffer[CMD_BUFFER];

    if (fd == -1)
        return EXIT_FAILURE;
    while (status > 0){
        status = read(socket_fd, buffer, CMD_BUFFER);
        if (status == -1)
            return EXIT_FAILURE;
        write(fd, buffer, status);
    }
    return EXIT_SUCCESS;
}
/*fd = open(new_path, O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);*/
int run_stor(char path[PATH_MAX],int control_socket, int socket_fd)
{
    pid_t p;

    p = fork();
    if (p < 0) {
      perror("fork fail");
      exit(1);
    }
    if (p == 0){
        dprintf(control_socket, "150 Opening data connection.\r\n");
        get_from_client(path, socket_fd);
        dprintf(control_socket, "226 Closing data connection.\r\n");
        close(socket_fd);
        exit(0);
    }
    close(socket_fd);
    return EXIT_SUCCESS;
}

int refresh_path(char path[PATH_MAX], const char *new_path, const server_t *serv);

int getnfilename(char *dest, const char *src, int n)
{
    int count = -1;
    int size_src = strlen(src) - 1;

    for (int i = size_src; src[i] != '\0' && src[i] != '/'; i--){
        count++;
    }
    if (count == -1)
        return 1;
    for (int i = size_src; i != 0; i--){
        if (src[i] == '/' || n == 0)
            return 1;
        dest[count] = src[i];
        count--;
        n--;
    }
    return 0;
}

int stor(ftp_t *ftp, int index, char *command)
{
    int status = 0;
    int nb_arg = get_number_arg(command);
    char path[PATH_MAX] = {0};
    char new_path[PATH_MAX] = {0};
    char file_name[256] = {0};

    
    if (is_connected(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    if (nb_arg > 2){
        dprintf(ftp->polling.fds[index].fd, "ftp 501 server cannot accept argument\r\n");
        return EXIT_SUCCESS;
    }
    strncpy(path, ftp->client[CLIENT_IDX(index)].wd, PATH_MAX);
    get_n_arg(command, new_path, 2);
    if (*new_path == '/')
        getnfilename(file_name, new_path, 256);
    refresh_path(path, (*new_path == '/') ? file_name : new_path, &ftp->server);
    status = accept_co(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd);
    if (status == DATA_NOT_READY || status == EXIT_FAILURE){
        if (status == DATA_NOT_READY) printf("Data not ready\n");
        return status;
    }
    run_stor(path, ftp->polling.fds[index].fd, ftp->client[CLIENT_IDX(index)].socket_fd);
    ftp->client[CLIENT_IDX(index)].datatransfer_mode = RESET_FLAG;
    return EXIT_SUCCESS;
}