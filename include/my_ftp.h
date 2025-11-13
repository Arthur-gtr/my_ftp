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
    #include <poll.h> 

    #include "utils.h"

    #define CRLF "\r\n"

    #define LIMIT_NAME 256
    #define MAX_SIMULTANEOUS_CONNEXION 5
    #define FDS_SERVER 0
    #define CLIENT_ID_MIN 1

    #define CLIENT_IDX(x) (x - 1)

    #define RUNNING 0
    #define CLOSE 1

    #define TIMEOUT -1


    typedef struct server_s {
        int server_fd;
        struct sockaddr_in addr;/*IPV4 type*/
        socklen_t addrlen;
    }server_t;
    
    /*CONNEXION*/
    #define RESET_FLAG 0
    #define USER_C (1 << 0)
    #define PASSW_C (1 << 1)
    #define CONNECTED (1 << 2)

    typedef struct ftp_command_s{

        char buffer[2048];
        int nb_crlf;
        
        /*Temporary contain one commnande and arg*/
        char command[1024];
        int size_cmd;
        
        /*The number of arg after the command ex[LIST, USER, ect...]*/
        int nb_arg;
        
        /*Start*/
        int pos;

        /*Charter after CRTF*/
        char garbage[2048];
    } ftp_command_t;

    typedef struct client_s{
        size_t alloc_client;
        size_t size;

        /*IPV4 type*/
        struct sockaddr_in addr;
        socklen_t addrlen;

        /*USER NAME*/
        char name[LIMIT_NAME];

        /*CONNECTION*/
        uint8_t connection;

        /*Work directory*/
        char *wd;

        /*command_buffer*/
        ftp_command_t cmd_info;
    }client_t;

    typedef struct polling_s{
        size_t alloc_pollfd;
        int nfds; 
        struct pollfd *fds;
    }polling_t;

    typedef struct ftp_s {
        server_t server;
        client_t *client;
        polling_t polling;
        int running;
    }ftp_t;

    int run_ftp(ftp_t *ftp);

    /*Commands*/
    int command_parsing(ftp_t *ftp, int index,char *command);
    int password(ftp_t *ftp, int index, char *command);
    int pwd(ftp_t *ftp, int index, char *command);

    /*FREE*/
    void destroy_ftp(ftp_t *ftp);

#endif /*MY_FTP_H*/