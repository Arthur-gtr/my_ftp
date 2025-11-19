/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** run_ftp
*/

#include <poll.h>

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <ctype.h>

#include "my_ftp.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>

int treat_overflow(char buffer[DATA_BUFFER], client_t *client, int fd)
{
    memset(client->cmd_info.buffer, 0, DATA_BUFFER);
    strncpy(buffer, client->cmd_info.buffer, DATA_BUFFER);
    dprintf(2, "Warning too much charcter stor in the command buffer\n");
    dprintf(fd, "500 Warning too much charcter stor \r\n");
    dprintf(fd, "in the command buffer next time you will be kick\r\n");
    return EXIT_FAILURE;
}

int get_data(ftp_t *ftp, int index)
{
    int status = 0;
    char buffer[DATA_BUFFER];

    status = read(ftp->polling.fds[index].fd, buffer, DATA_BUFFER);
    if (status == -1)
        return EXIT_FAILURE;
    if (status == 0){
        close(ftp->polling.fds[index].fd);
        ftp->polling.fds[index].fd = -1;
        return EXIT_FAILURE;
    }
    buffer[status] = '\0';
    if (status + strlen(ftp->client[CLIENT_IDX(index)].cmd_info.buffer) > 2048)
        return treat_overflow(buffer,
            &ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd);
    strcat(ftp->client[CLIENT_IDX(index)].cmd_info.buffer, buffer);
    return EXIT_SUCCESS;
}
