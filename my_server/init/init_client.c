/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** init_client
*/

/*Network*/
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdlib.h>

#include "my_ftp.h"

int init_client(client_array_t *client_tab)
{
    client_tab->size = 0;
    client_tab->alloc = 1;
    return EXIT_SUCCESS;
}
