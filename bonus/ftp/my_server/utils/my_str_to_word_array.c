/*
** EPITECH PROJECT, 2024
** my_str_to_word_array
** File description:
** to separate word on a array
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

static int count_word(char *word)
{
    int number_of_word = 0;

    if (word[0] == '\0')
        return 0;
    for (int i = 0; word[i + 1] != '\0'; i++)
        if (((word[i] == 30 || i == 0) && word[i + 1] != 30))
            number_of_word++;
    if (word[0] != 30 && (word[1] == 30 || word[1] == '\0'))
        number_of_word++;
    return number_of_word;
}

static char *removed_separator(char *str, char *removed_separator)
{
    for (int char_of_str = 0; str[char_of_str] != '\0'; char_of_str++)
        for (int find_separator = 0; removed_separator[find_separator] != '\0';
                find_separator++)
            str[char_of_str] = (str[char_of_str] ==
                removed_separator[find_separator]) ? 30 : str[char_of_str];
    return str;
}

static int first_word(char *str, int what_word)
{
    int where_is_first_word = 0;
    int find = 0;

    if (str[0] != 30 && what_word == 0)
        return 0;
    if (str[0] != 30 && what_word != 0)
        find++;
    for (int i = 0; str[i + 1] != '\0' && (find <= what_word); i++){
        if (str[i] == 30 && str[i + 1] != 30){
            where_is_first_word = i;
            find++;
        }
    }
    return where_is_first_word + 1;
}

static char *str_in_array(char *str, int what_word)
{
    int wher_word = first_word(str, what_word);
    int len_new_str = 0;
    char *new_str;

    for (int i = wher_word; (str[i] != 30 && str[i + 1] != 30)
        && str[i + 1] != '\0'; i++)
        len_new_str++;
    new_str = malloc(sizeof(char) * (len_new_str + 2));
    if (new_str == NULL)
        return NULL;
    new_str[len_new_str + 1] = '\0';
    new_str[len_new_str] = '\0';
    for (int i = wher_word; (str[i] != 30 && str[i + 1] != 30)
        && str[i + 1] != '\0'; i++)
        new_str[i - wher_word] = str[i];
    if (new_str[len_new_str] == '\0' && str[len_new_str + wher_word] != 30)
        new_str[len_new_str] = str[len_new_str + wher_word];
    return new_str;
}

static char **init_array(char *str, int number_of_word)
{
    char **array = malloc(sizeof(char *) * (number_of_word + 1));

    if (array == NULL)
        return NULL;
    array[number_of_word] = NULL;
    for (int i = 0; i != number_of_word; i++){
        array[i] = str_in_array(str, i);
        if (array[i] == NULL)
            return NULL;
    }
    return array;
}

char **my_str_to_word_array(char *str, char *separator)
{
    char **array;
    char *cpy_str = strdup(str);
    int number_of_word;

    if (str == NULL || separator == NULL)
        return NULL;
    if (cpy_str == NULL)
        return NULL;
    cpy_str = removed_separator(cpy_str, separator);
    number_of_word = count_word(cpy_str);
    array = init_array(cpy_str, number_of_word);
    free(cpy_str);
    if (array == NULL)
        return NULL;
    return array;
}
