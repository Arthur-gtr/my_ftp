/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** LIST
*/
#include "my_ftp.h"
#include "command.h"

#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int fill_ls_buffer(int fd, server_t *serv,
    const char client_wd[PATH_MAX], char dest[PATH_MAX + 10])
{
    memset(dest, 0, PATH_MAX + 10);
    if (serv->size_wd + strlen(client_wd) + 5 >= PATH_MAX){
        dprintf(fd, "400 wd overflow\r\n");
        return EXIT_FAILURE;
    }
    strncpy(dest, "ls -l ", PATH_MAX);
    strncat(dest, serv->serv_wd, PATH_MAX);
    strncat(dest, client_wd, PATH_MAX);
    printf("Cmd buffer: %s\n", dest);
    return EXIT_SUCCESS;
}

void run_ls(int fd, server_t *serv, const char client_wd[PATH_MAX])
{
    char cmd[PATH_MAX + 10];
    FILE *fp = NULL;
    char buffer[PATH_MAX];

    if (fill_ls_buffer(fd, serv, client_wd, cmd) == EXIT_FAILURE)
        return;
    fp = popen(cmd, "r");
    if (fp == NULL)
        return;
    while (fgets(buffer, sizeof(buffer), fp) != NULL){
        buffer[strcspn(buffer, "\n")] = '\0';
        dprintf(fd, "%s\r\n", buffer);
    }
    pclose(fp);
}

void give_list_to_the_client(int fd, int control_fd,
    server_t *serv, const char client_wd[PATH_MAX])
{
    pid_t p;

    p = fork();
    if (p < 0) {
        perror("fork fail");
        exit(1);
    }
    if (p == 0){
        run_ls(fd, serv, client_wd);
        dprintf(control_fd, "226 Closing data connection.\r\n");
        close(fd);
        exit(0);
    }
    close(fd);
    return;
}

int refresh_path(char path[PATH_MAX],
    const char *new_path, const server_t *serv)
{
    int count = 0;

    if (*new_path == '/'){
        memset(path, 0, PATH_MAX);
        strncpy(path, new_path, PATH_MAX);
        return EXIT_SUCCESS;
    }
    if (realpath(new_path, path) == NULL)
        return EXIT_FAILURE;
    for (int i = serv->size_wd; path[i] != 0; i++){
        path[count] = path[i];
        count++;
    }
    for (int i = count; path[i] != '\0'; i++)
        path[i] = '\0';
    return EXIT_SUCCESS;
}

static
int init_ls(ftp_t *ftp, int nb_arg, int index)
{
    if (is_connected(&ftp->client[CLIENT_IDX(index)],
        ftp->polling.fds[index].fd) == false)
        return EXIT_FAILURE;
    if (nb_arg > 2){
        dprintf(ftp->polling.fds[index].fd, ARG_501);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int list(ftp_t *ftp, int index, char *command)
{
    int status = 0;
    int nb_arg = get_number_arg(command);
    char path[PATH_MAX] = {0};
    char new_path[PATH_MAX] = {0};

    strncpy(path, ftp->client[CLIENT_IDX(index)].wd, PATH_MAX);
    if (init_ls(ftp, nb_arg, index) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    status = accept_co(&ftp->client[CLIENT_IDX(index)],
        ftp->polling.fds[index].fd);
    if (status == DATA_NOT_READY || status == EXIT_FAILURE)
        return status;
    if (nb_arg == 2){
        get_n_arg(command, new_path, 2);
        refresh_path(path, new_path, &ftp->server);
    }
    dprintf(ftp->polling.fds[index].fd, "150 Opening data connection.\r\n");
    give_list_to_the_client(ftp->client[CLIENT_IDX(index)].socket_fd,
        ftp->polling.fds[index].fd, &ftp->server, path);
    return EXIT_SUCCESS;
}
