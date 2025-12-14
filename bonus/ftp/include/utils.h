/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/


#ifndef UTIILS_H
    #define UTIILS_H
    #define IS_EMPTY(x) (x == '\0')

    #include <stdbool.h>

int reterr(char *str);
char **my_str_to_word_array(char *str, char *separator);
int is_valid(char c);
bool is_file(char *path);
void prepend(char *dest, const char *src);
#endif /*UTIILS_H*/
