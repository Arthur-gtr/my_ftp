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

int init_client(client_t *client)
{
    client->size = 0;
    client->alloc_client = 1;
    return EXIT_SUCCESS;
}