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

int send_to_client(char path[PATH_MAX], int socket_fd)
{
    int status = 1;
    int fd = open(path, O_RDONLY);
    char buffer[CMD_BUFFER];

    if (fd == -1)
        return EXIT_FAILURE;
    while (status > 0){
        status = read(fd, buffer, CMD_BUFFER);
        if (status == -1)
            return EXIT_FAILURE;
        write(socket_fd, buffer, status);
    }
    return EXIT_SUCCESS;
}

int run_retr(char path[PATH_MAX],int control_socket, int socket_fd)
{
    pid_t p;

    p = fork();
    if (p < 0) {
      perror("fork fail");
      exit(1);
    }
    if (p == 0){
        dprintf(control_socket, "150 Opening data connection.\r\n");
        send_to_client(path, socket_fd);
        dprintf(control_socket, "226 Closing data connection.\r\n");
        close(socket_fd);
        exit(0);
    }
    close(socket_fd);
    return EXIT_SUCCESS;
}

int retr(ftp_t *ftp, int index, char *command)
{
    int status = 0;
    int nb_arg = get_number_arg(command);
    char path[PATH_MAX] = {0};
    char new_path[PATH_MAX] = {0};

    printf("Start Retr\n");
    if (is_connected(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    if (nb_arg > 2){
        dprintf(ftp->polling.fds[index].fd, "ftp 501 server cannot accept argument\r\n");
        return EXIT_SUCCESS;
    }
    get_n_arg(command, new_path, 2);
    if (realpath(new_path, path) == NULL){
        dprintf(ftp->polling.fds[index].fd, "550 Failed to open file.\r\n");
        return EXIT_SUCCESS;
    }
    if (is_file(path) == false){
        dprintf(ftp->polling.fds[index].fd, "550 Must be a file.\r\n");
        return EXIT_SUCCESS;
    }
    status = accept_co(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd);
    if (status == DATA_NOT_READY || status == EXIT_FAILURE){
        if (status == DATA_NOT_READY) printf("Data not ready\n");
        return status;
    }
    
    run_retr(path, ftp->polling.fds[index].fd, ftp->client[CLIENT_IDX(index)].socket_fd);
    ftp->client[CLIENT_IDX(index)].datatransfer_mode = RESET_FLAG;
    return EXIT_SUCCESS;
}