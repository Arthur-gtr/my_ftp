#include "command.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>



int len_array(char **array)
{
    int i = 0;

    for (; array[i] != NULL; i++);
    return i;
}

int test(ftp_t *ftp, int index, char *command)
{
    write(ftp->polling.fds[index].fd, "502 command foud but not implemented\r\n", 38);
    return EXIT_SUCCESS;
}

int quit(ftp_t *ftp, int index, char *command)
{
   write(ftp->polling.fds[index].fd , "221 quit\r\n", 9);
   return EXIT_SUCCESS;
}

int syst(ftp_t *ftp, int index, char *command)
{
   write(ftp->polling.fds[index].fd , "215 UNIX Type: L8\r\n", 19);
   return EXIT_SUCCESS;
}

int paswd_valid(char *paswd)
{
    for (int i = 0; paswd[i] != 0; i++){
        if (strncmp(&paswd[i], CRLF, CRLF_SZ) == 0){
            printf("PASS quit on index %d\n", i);
            return EXIT_SUCCESS;
        }
        if (is_valid(paswd[i])){
            printf("Char non valide %c", paswd[i]);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int password(ftp_t *ftp, int index, char *command)
{
    command += 4;
    if (get_number_arg(command) > 2){
        dprintf(ftp->polling.fds[index].fd, "ftp 501 server cannot accept argument\r\n");
        return EXIT_SUCCESS;
    }
    if (!(USER_C & ftp->client[CLIENT_IDX(index)].connection)){
        printf("Username don't set: %d\n", ftp->client[CLIENT_IDX(index)].connection);
        write(ftp->polling.fds[index].fd, "530 Username don't set...\r\n", 27);
        return EXIT_SUCCESS;
    }
    if (paswd_valid(command) == EXIT_SUCCESS){
        ftp->client[CLIENT_IDX(index)].connection |= PASSW_C;
        ftp->client[CLIENT_IDX(index)].connection |= CONNECTED;
        write(ftp->polling.fds[index].fd, "230 Password OK: Welcome aboard captain\r\n", 41);
        return EXIT_SUCCESS;
    }
    write(ftp->polling.fds[index].fd, "501 Password error unvalid character...\r\n", 27);
    return EXIT_SUCCESS;
}

int command_parsing(ftp_t *ftp, int index)
{
    int status = -2;
    /*Get first arg*/
    for (size_t i = 0; i != sz_tab_cmd; i++){
        if (strncmp(command_tab[i].command_name, ftp->client[CLIENT_IDX(index)].cmd_info.command, command_tab[i].size) == 0){
            status = command_tab[i].funct(ftp, index, ftp->client[CLIENT_IDX(index)].cmd_info.command);
        }
        if (status == MALLOC_FAILED || status == EXIT_SUCCESS){
            return status;
        }
    }
    write(ftp->polling.fds[index].fd, "500 Syntax error, command unrecognized.\r\n", 41);
    printf("ftp::command: %s :", ftp->client[CLIENT_IDX(index)].cmd_info.command);
    return reterr("500 Syntax error, command unrecognized.");
}
