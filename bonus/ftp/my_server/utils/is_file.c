/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** is_file
*/

#include <sys/stat.h>
#include <stdbool.h>

bool is_file(char *path)
{
    struct stat path_stat;

    stat(path, &path_stat);
    if (S_ISREG(path_stat.st_mode))
        return true;
    return false;
}
