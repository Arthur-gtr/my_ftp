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

int password(ftp_t *ftp, int index, char *command)
{
    command += 5;
    printf("Password %s\n", command);
    if (!(USER_C & ftp->client[CLIENT_IDX(index)].connection)){
        write(ftp->polling.fds[index].fd, "530 Username don't set...\r\n", 27);
        return EXIT_FAILURE;
    }
    /*if (!(parse_password){
        write(ftp->polling.fds[index].fd, "501 Password error unvalid character...\r\n", 27);
        return EXIT_FAILURE;
    }*/
    if (strcmp(command, "\r\n") == 0){
        ftp->client[CLIENT_IDX(index)].connection |= PASSW_C;
        ftp->client[CLIENT_IDX(index)].connection |= CONNECTED;
        write(ftp->polling.fds[index].fd, "230 Password OK: Welcome aboard captain\r\n", 41);
        return EXIT_SUCCESS;
    }
    write(ftp->polling.fds[index].fd, "332 Need account for login.\r\n", 29);
    return EXIT_SUCCESS;
}

int command_parsing(ftp_t *ftp, int index, char *command)
{
    int status = -1;
    /*Get first arg*/

    for (size_t i = 0; i != sz_tab_cmd; i++){
        if (strncmp(command_tab[i].command_name, command, command_tab[i].size) == 0){
            status = command_tab[i].funct(ftp, index, command);
        }
        if (status != -1)
            return status;
    }
    write(ftp->polling.fds[index].fd, "500 Syntax error, command unrecognized.\r\n", 42);
    return reterr("500 Syntax error, command unrecognized.");
}
