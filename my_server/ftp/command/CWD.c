/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** CWD
*/
#include "my_ftp.h"
#include "command.h"

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


int cwd(ftp_t *ftp, int index, char *command)
{
    char path_arg[CMD_BUFFER] = {0};
    char new_wc[PATH_MAX] = {0};

    if (is_connected(&ftp->client[CLIENT_IDX(index)], ftp->polling.fds[index].fd) == false)
        return EXIT_SUCCESS;
    get_n_arg(command, path_arg, 2);
    /*Si le path_arg commence par / alors je dois le concaténer par rapport au server->wd*/
    if ((*path_arg) == '/'){
        printf("Add pre loc\n");
        prepend(path_arg, ftp->server.serv_wd);
    }
    printf("Number of arg == %d\n", get_number_arg(command));
    printf("Path Arg: %s\n", path_arg);
    print_visible(path_arg);
    if (chdir(path_arg) == -1){
        dprintf(ftp->polling.fds[index].fd, "500 chdir failed.\r\n");
        /*CHECK ERNO POUR VOIR SI LE PATH EXISTE*/
        return EXIT_SUCCESS;
    }
    if (getcwd(new_wc, sizeof(new_wc)) == NULL){
        dprintf(ftp->polling.fds[index].fd, "500 getcwd failed\r\n");
        return EXIT_SUCCESS;
    }
    if (strlen(new_wc) < ftp->server.size_wd){
        memset(new_wc, 0, sizeof(new_wc));
        strncpy(new_wc, ftp->server.serv_wd, sizeof(new_wc));
        strncat(new_wc, ftp->client[CLIENT_IDX(index)].wd, sizeof(new_wc) - strlen(new_wc));
        printf("Old wd: %s\n", new_wc);
        chdir(new_wc);
        dprintf(ftp->polling.fds[index].fd, "500 Directory successfully changed.\r\n");
        return EXIT_SUCCESS;
    }
    int count = 0;
    for (int i = ftp->server.size_wd; new_wc[i] != '\0'; i++){
        ftp->client[CLIENT_IDX(index)].wd[count] = new_wc[i];
        count++;
    }
    ftp->client[CLIENT_IDX(index)].wd[count + 1] = '\0';
    dprintf(ftp->polling.fds[index].fd, "250 Directory successfully changed.\r\n");
    /*je dois get le premier arg dans un buffer*/
    /*On vas faire une simulation du déplacment via chdir puis si il y a une erreur on n' écrase pas pas le path en mémoire*/
    return 0;
}