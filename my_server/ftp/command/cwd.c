/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** CWD
*/
#include "my_ftp.h"
#include "command.h"
#include "utils.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <string.h>

void prepend(char *dest, const char *src)
{
    size_t len_src = strlen(src);
    size_t len_dest = strlen(dest);

    memmove(dest + len_src, dest, len_dest + 1);
    memcpy(dest, src, len_src);
}

static
int reset_cwd(char *new_wc, ftp_t *ftp, int index)
{
    memset(new_wc, 0, strlen(new_wc));
    strncpy(new_wc, ftp->server.serv_wd, PATH_MAX);
    strncat(new_wc, ftp->client[CLIENT_IDX(index)].wd,
        PATH_MAX - strlen(new_wc));
    chdir(new_wc);
    dprintf(ftp->polling.fds[index].fd, "550 Failed to change directory.\r\n");
    return EXIT_SUCCESS;
}

static
void change_cwd(ftp_t *ftp, int index, char *new_wc)
{
    int count = 0;

    memset(ftp->client[CLIENT_IDX(index)].wd, 0, PATH_MAX);
    for (int i = ftp->server.size_wd; new_wc[i] != '\0'; i++){
        ftp->client[CLIENT_IDX(index)].wd[count] = new_wc[i];
        count++;
    }
    ftp->client[CLIENT_IDX(index)].wd[count] = '\0';
    if (IS_EMPTY(*ftp->client[CLIENT_IDX(index)].wd))
        *ftp->client[CLIENT_IDX(index)].wd = '/';
    dprintf(ftp->polling.fds[index].fd,
        "250 Directory successfully changed.\r\n");
}

static
int init_cwd(ftp_t *ftp, char *command, int index)
{
    if (is_connected(&ftp->client[CLIENT_IDX(index)],
        ftp->polling.fds[index].fd) == false)
        return EXIT_FAILURE;
    if (get_number_arg(command) > 2){
        dprintf(ftp->polling.fds[index].fd,
            "ftp 501 server cannot accept argument\r\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int cwd(ftp_t *ftp, int index, char *command)
{
    char path_arg[CMD_BUFFER] = {0};
    char new_wc[PATH_MAX] = {0};

    if (init_cwd(ftp, command, index) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    get_n_arg(command, path_arg, 2);
    if ((*path_arg) == '/')
        prepend(path_arg, ftp->server.serv_wd);
    if (chdir(path_arg) == -1 || getcwd(new_wc, sizeof(new_wc)) == NULL){
        dprintf(ftp->polling.fds[index].fd,
            "550 Failed to change directory.\r\n");
        return EXIT_SUCCESS;
    }
    if (strlen(new_wc) < ftp->server.size_wd)
        return reset_cwd(new_wc, ftp, index);
    change_cwd(ftp, index, new_wc);
    return 0;
}
