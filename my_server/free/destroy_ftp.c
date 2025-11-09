/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** init_ftp
*/

/*Network*/
#include <netdb.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 

#include <stdlib.h>
#include <unistd.h>

#include "my_ftp.h"

void destroy_server(server_t *server)
{
    close(server->server_fd);
}

void destroy_polling(polling_t *polling)
{
    free(polling->fds);
}

void destroy_ftp(ftp_t *ftp)
{
    destroy_server(&ftp->server);
    destroy_polling(&ftp->polling);
}
