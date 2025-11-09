/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** reterr
*/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int reterr(char *str)
{
    write(2, str, strlen(str));
    write(2, "\n", 1);
    return EXIT_FAILURE;
}