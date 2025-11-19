/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** is_valid
*/

#include <ctype.h>

int is_valid(char c)
{
    return isalpha(c) || isdigit(c) || ispunct(c);
}
