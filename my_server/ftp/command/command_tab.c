#include "command.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define CACA &test

int syst(ftp_t *ftp, int index, char *command);
int feat(ftp_t *ftp, int index, char *command);
int user(ftp_t *ftp, int index, char *command);
int password(ftp_t *ftp, int index, char *command);



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

command_t command_tab[] = {
    {"CDUP", false, 0, CACA},
    {"CWD",  false, 1, CACA},
    {"DELE", false, 1, CACA},
    {"HELP", false, 0, CACA},
    {"LIST", false, 0, CACA},
    {"NOOP", false, 0, CACA},
    {"PASS", false, 1, &password},
    {"PASV", false, 0, CACA},
    {"PORT", false, 1, CACA},
    {"PWD",  false, 0, CACA},
    {"QUIT", false, 0, CACA},
    {"RETR", false, 1, CACA},
    {"STOR", false, 1, CACA},
    {"USER", false, 1, &user},
    {"SYST", false, 1, &syst},
    {"FEAT", false, 1, &feat},
};

const size_t sz_tab_cmd = sizeof(command_tab) / sizeof(*command_tab);

void print_visible(const char *s) {
    while (*s) {
        switch (*s) {
            case '\n': printf("\\n"); break;
            case '\t': printf("\\t"); break;
            case '\r': printf("\\r"); break;
            case '\\': printf("\\\\"); break;
            case ' ': printf("X"); break;
            default:
                if ((unsigned char)*s < 32 || (unsigned char)*s > 126)
                    printf("\\x%02x", (unsigned char)*s);
                else
                    putchar(*s);
        }
        s++;
    }
    putchar('\n');
}

int feat(ftp_t *ftp, int index, char *command)
{
   write(ftp->polling.fds[index].fd , "211 End\r\n", 9);
   return EXIT_SUCCESS;
}

int syst(ftp_t *ftp, int index, char *command)
{
   write(ftp->polling.fds[index].fd , "215 UNIX Type: L8\r\n", 19);
   return EXIT_SUCCESS;
}

int password(ftp_t *ftp, int index, char *command)
{
    printf("Password\n");
    command += 5;
    if (!(USER_C & ftp->client[index].connection)){
        write(ftp->polling.fds[index].fd, "530 Username don't set...\r\n", 27);
        return EXIT_FAILURE;
    }
    /*if (!(parse_password){
        write(ftp->polling.fds[index].fd, "501 Password error unvalid character...\r\n", 27);
        return EXIT_FAILURE;
    }*/
    if (strcmp(command, "\r\n") == 0){
        ftp->client[index].connection |= PASSW_C;
        ftp->client[index].connection |= CONNECTED;
        write(ftp->polling.fds[index].fd, "230 Password OK: Welcome aboard captain\r\n", 41);
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

int user(ftp_t *ftp, int index, char *command)
{
    printf("buffer: %s\n", command);
    command += strlen(command_tab[0].command_name) + 1;
    if (strlen(command) > LIMIT_NAME){
        ftp->client[index].connection = 0;
        write(ftp->polling.fds[index].fd, "501 Username too long\r\n", 23);
        return EXIT_FAILURE;
    }
    /*TEST si il y a des \n ou des char spéciaux*/
    print_visible(command);
    printf("%d\n", strcmp(command, "anonymous\r\n"));
    if (strcmp(command, "anonymous\r\n") == 0){
        ftp->client[index].connection |= USER_C;
        write(ftp->polling.fds[index].fd, "331 Username Accepted\r\n", 23);
        return EXIT_SUCCESS;
    }
    write(ftp->polling.fds[index].fd, "530 Uknowing name...\r\n", 23);
    return EXIT_FAILURE;
}

int command_parsing(ftp_t *ftp, int index,char *command)
{
    int status = -1;

    for (size_t i = 0; i != sz_tab_cmd; i++){
        if (strncmp(command_tab[i].command_name, command, 4) == 0){
            printf("Command detected: %s\n", command_tab[i].command_name);
            status = command_tab[i].funct(ftp, index, command);
        }
        if (status != -1)
            return status;
    }
    write(ftp->polling.fds[index].fd, "502 command not found\r\n", 23);
    return reterr("ERROR COMMAND NOT FOUDE");
}
