/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** parsing_command
*/

#include <stdlib.h>
#include <my_ftp.h>
#include <stdio.h>
#include <string.h>
#define CMD_SEPARATOR(c_cmd) (c_cmd == ' ' || c_cmd == '\t')

int get_number_arg(char *buffer)
{
    int nb_arg = (CMD_SEPARATOR(*buffer)) ? 0 : 1;
    bool is_space = (CMD_SEPARATOR(*buffer)) ? true : false;

    for (int i = 0; buffer[i] != '\0'; i++){
        if (CMD_SEPARATOR(buffer[i])){
            is_space = true;
            continue;
        }
        if (is_valid(buffer[i]) && is_space == true){
            is_space = false;
            nb_arg++;
        }
    }
    return nb_arg;
}

int get_pos_arg(const char *buffer, int n)
{
    int nb_arg = (CMD_SEPARATOR(*buffer)) ? 0 : 1;
    bool is_space = (CMD_SEPARATOR(*buffer)) ? true : false;

    for (int i = 0; buffer[i] != '\0'; i++){
        if (nb_arg == n)
            return i - 1;
        if (CMD_SEPARATOR(buffer[i])){
            is_space = true;
            continue;
        }
        if (is_valid(buffer[i]) && is_space == true){
            is_space = false;
            nb_arg++;
        }
    }
    return -1;
}

int get_n_arg(const char *buffer, char *dest, int n)
{
    int idx_begin = get_pos_arg(buffer, n);;
    int count = 0;

    for(int i = idx_begin; buffer[i] != '\t' && buffer[i] != ' ' && buffer[i] != 0; i++ ){
        if (strncmp(&buffer[i], CRLF, 2) == 0)
            return count;
        dest[count] = buffer[i];
        count++;   
    }
    printf("%s\n", dest);
    return count;
}