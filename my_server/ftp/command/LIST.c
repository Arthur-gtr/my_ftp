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



int accept_out_co(client_t *client)
{
    client->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    //int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    //inet_pton(AF_INET, "192.168.1.42", &addr.sin_addr);
    //Remplir en fonction de la structur qui vas servir a PORT
    client->socket_fd = accept(client->pasv_fd, (struct sockaddr *)&client->addr, &client->addrlen);
    close(client->pasv_fd);
    if (client->socket_fd == -1)
        return MALLOC_FAILED;

    return 0;
}

#define DATA_NOT_READY -1

int accept_in_co(client_t *client)
{
    client->socket_fd = accept(client->pasv_fd, (struct sockaddr *)&client->addr, &client->addrlen);
    if (client->socket_fd == -1)
        return MALLOC_FAILED;
    return EXIT_SUCCESS;
}

int fill_ls_buffer(int fd, server_t *serv, const char client_wd[PATH_MAX], char dest[PATH_MAX + 10])
{
    memset(dest,0, PATH_MAX + 10);
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
    FILE *fp = NULL;/*popen("ls -l", "r");*/
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

void give_list_to_the_client(int fd, int control_fd, server_t *serv, const char client_wd[PATH_MAX])
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
        exit(1);
    }
    close(fd);
    return;
}

int accept_co(client_t *client, int fd)
{
    if (client->datatransfer_ready == false){    
        dprintf(fd, "425 Can't open data connection.\r\n");
        return EXIT_FAILURE;
    }
    client->datatransfer_ready = false;
    if (client->datatransfer_mode == PASV)
        return accept_in_co(client);
    printf("PASV NOT EXECUTE: %d\n", client->datatransfer_mode);
    return DATA_NOT_READY;
}

int list(ftp_t *ftp, int index, char *command)
{
    int status = 0;

    printf("Start LIST\n");
    if (is_connected(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    status = accept_co(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd);
    if (status == DATA_NOT_READY || status == EXIT_FAILURE){
        if (status == DATA_NOT_READY) printf("Data not ready\n");
        return status;
    }
    dprintf(ftp->polling.fds[index].fd, "150 Opening data connection.\r\n");
    give_list_to_the_client(ftp->client[CLIENT_IDX(index)].socket_fd, ftp->polling.fds[index].fd, &ftp->server, ftp->client[CLIENT_IDX(index)].wd);
    ftp->client[CLIENT_IDX(index)].datatransfer_mode = 0;
    return EXIT_SUCCESS;
}