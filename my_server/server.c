/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** server
*/

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

#include "my_ftp.h"

#include "init.h"

int my_ftp(char *port, char *path)
{
    ftp_t ftp;

    if (init_ftp(&ftp, port, path) == EXIT_FAILURE)
        return EXIT_FAILURE;
    if (run_ftp(&ftp) == EXIT_FAILURE){
        destroy_ftp(&ftp);
        return EXIT_FAILURE;
    }
    destroy_ftp(&ftp);
    return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
    (void)(ac);
    (void)(av);
    if (ac < 3)
        return 84;
    return my_ftp(av[1], av[2]);
}
