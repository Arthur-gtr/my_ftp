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
    #include <stdbool.h>
    #include <limits.h>

    #include "utils.h"

    #define CRLF "\r\n"
    #define CRLF_SZ 2

    #define LIMIT_NAME 256
    #define MAX_SIMULTANEOUS_CONNEXION 5
    #define FDS_SERVER 0
    #define CLIENT_ID_MIN 1

    #define CLIENT_IDX(x) (x - 1)

    #define RUNNING 0
    #define CLOSE 1

    #define TIMEOUT -1

    #define MALLOC_FAILED -1

    /*CONNEXION*/
    #define RESET_FLAG 0
    #define USER_C (1 << 0)
    #define PASSW_C (1 << 1)
    #define CONNECTED (1 << 2)

    #define DATA_BUFFER 2048
    #define CMD_BUFFER 1024

    #ifndef PATH_MAX     
    #define PATH_MAX 4096
    #endif 

     typedef struct server_s {
        int server_fd;
        struct sockaddr_in addr;/*IPV4 type*/
        socklen_t addrlen;
        char serv_wd[PATH_MAX];
        int size_wd;
    }server_t;
    

    typedef struct ftp_command_s{

        char buffer[DATA_BUFFER];
        int nb_crlf;
        
        /*Temporary contain one commnande and arg*/
        char command[CMD_BUFFER];
        int size_cmd;

        /*The number of arg after the command ex[LIST, USER, ect...]*/
        int nb_arg;
        bool garbage_status;

        /*Start*/
        int pos;

        /*Charter after CRTF*/
        char garbage[DATA_BUFFER];
    } ftp_command_t;

    typedef struct client_s{
        /*Tab struct*/
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
        char wd[PATH_MAX];
        int size_wd;

        /*command_buffer*/
        ftp_command_t cmd_info;

        /*PASV mode PORT mode or nothing*/
        uint8_t datatransfer_mode;
        bool datatransfer_ready;

        /*Passive mode*/
        struct sockaddr_in addr_pasv;
        socklen_t addrlen_pasv;

        int pasv_fd;
        int socket_fd;
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

    /*Connection*/
    int add_user(ftp_t *ftp);

    /*Command utils*/
    int command_parsing(ftp_t *ftp, int index);
    bool is_connected(client_t *client, int fd);
    /*Commands*/
    int password(ftp_t *ftp, int index, char *command);
    int pwd(ftp_t *ftp, int index, char *command);
    int pasv(ftp_t *ftp, int index, char *command);

    /*FREE*/
    void destroy_ftp(ftp_t *ftp);

#endif /*MY_FTP_H*/