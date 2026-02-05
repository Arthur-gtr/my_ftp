/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** PASS
*/
#include "my_ftp.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int paswd_valid(char *paswd)
{
    for (int i = 0; paswd[i] != 0; i++){
        if (strncmp(&paswd[i], CRLF, CRLF_SZ) == 0)
            return EXIT_SUCCESS;
        if (is_valid(paswd[i]))
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int password(ftp_t *ftp, int index, char *command)
{
    command += 4;
    if (get_number_arg(command) > 2){
        dprintf(ftp->polling.fds[index].fd, ARG_501);
        return EXIT_SUCCESS;
    }
    if (!(USER_C & ftp->client_tab.client[CLIENT_IDX(index)].connection)){
        dprintf(ftp->polling.fds[index].fd, "530 Username don't set...\r\n");
        return EXIT_SUCCESS;
    }
    if (paswd_valid(command) == EXIT_SUCCESS){
        ftp->client_tab.client[CLIENT_IDX(index)].connection |= PASSW_C;
        ftp->client_tab.client[CLIENT_IDX(index)].connection |= CONNECTED;
        dprintf(ftp->polling.fds[index].fd,
            "230 User logged in, proceed.\r\n");
        return EXIT_SUCCESS;
    }
    dprintf(ftp->polling.fds[index].fd,
        "501 Password error unvalid character...\r\n");
    return EXIT_SUCCESS;
}
