/*
** EPITECH PROJECT, 2025
** bonus
** File description:
** send_to_gui
*/
#include "my_ftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void run_gui(__attribute_maybe_unused__ ftp_t *ftp, int fd)
{
    char buffer[1024];
    read(fd, buffer, 1024);
    dprintf(fd, "500\r\n");
}

void give_info_to_gui(ftp_t *ftp, int fd)
{
    pid_t p;

    p = fork();
    if (p < 0) {
        perror("fork fail");
        exit(1);
    }
    if (p == 0){
        run_gui(ftp, fd);  
        close(fd);
        exit(0);
    }
    return;
}

int send_to_gui(ftp_t *ftp, int index)
{
    give_info_to_gui(ftp, ftp->polling.fds[index].fd);
    return 0;
}
