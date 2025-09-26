/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/


#ifndef MY_FTP_H
    #define MY_FTP_H

    #include <netdb.h> 
    #include <netinet/in.h> 
    #include <sys/socket.h> 
    #define LIMIT_NAME 256

    typedef struct server_s {
        int server_fd;
        struct sockaddr_in addr;/*IPV4 type*/
        socklen_t addrlen;
    }server_t;
    
    typedef struct client_s{
        int id;/*Start at 1*/
        struct sockaddr_in addr;/*IPV4 type*/
        socklen_t addrlen;
        char name[LIMIT_NAME];
    }*client_t;

    typedef struct ftp_s {
        server_t server;
        client_t *client;
    }ftp_t;

#endif /*MY_FTP_H*/