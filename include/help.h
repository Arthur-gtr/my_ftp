/*
** EPITECH PROJECT, 2025
** minishell
** File description:
** my
*/


#ifndef HELP_H
    #define HELP_H

    #define HELP_USER_0 "USER <SP> <username> <CRLF>   : "
    #define HELP_USER_1 "Specify user for authentication\r\n"
    #define HELP_USER   HELP_USER_0 HELP_USER_1

    #define HELP_PASS_0 "PASS <SP> <password> <CRLF>   : "
    #define HELP_PASS_1 "Specify password for authentication\r\n"
    #define HELP_PASS   HELP_PASS_0 HELP_PASS_1

    #define HELP_CWD_0  "CWD  <SP> <pathname> <CRLF>   : "
    #define HELP_CWD_1  "Change working directory\r\n"
    #define HELP_CWD    HELP_CWD_0 HELP_CWD_1

    #define HELP_CDUP_0 "CDUP <CRLF>                   : "
    #define HELP_CDUP_1 "Change working directory to parent directory\r\n"
    #define HELP_CDUP   HELP_CDUP_0 HELP_CDUP_1

    #define HELP_QUIT_0 "QUIT <CRLF>                   : "
    #define HELP_QUIT_1 "Disconnection\r\n"
    #define HELP_QUIT   HELP_QUIT_0 HELP_QUIT_1

    #define HELP_DELE_0 "DELE <SP> <pathname> <CRLF>   : "
    #define HELP_DELE_1 "Delete file on the server\r\n"
    #define HELP_DELE   HELP_DELE_0 HELP_DELE_1

    #define HELP_PWD_0  "PWD  <CRLF>                   : "
    #define HELP_PWD_1  "Print working directory\r\n"
    #define HELP_PWD    HELP_PWD_0 HELP_PWD_1

    #define HELP_PASV_0 "PASV <CRLF>                   : "
    #define HELP_PASV_1 "Enable \"passive\" mode for data transfer\r\n"
    #define HELP_PASV   HELP_PASV_0 HELP_PASV_1

    #define HELP_PORT_0 "PORT <SP> <host-port> <CRLF>  : "
    #define HELP_PORT_1 "Enable \"active\" mode for data transfer\r\n"
    #define HELP_PORT   HELP_PORT_0 HELP_PORT_1

    #define HELP_HELP_0 "HELP [<SP> <string>] <CRLF>   : "
    #define HELP_HELP_1 "List available commands\r\n"
    #define HELP_HELP   HELP_HELP_0 HELP_HELP_1

    #define HELP_NOOP_0 "NOOP <CRLF>                   : "
    #define HELP_NOOP_1 "Do nothing\r\n"
    #define HELP_NOOP   HELP_NOOP_0 HELP_NOOP_1

    #define HELP_RETR_0 "RETR <SP> <pathname> <CRLF>   : "
    #define HELP_RETR_1 "Download file from server to client\r\n"
    #define HELP_RETR   HELP_RETR_0 HELP_RETR_1

    #define HELP_STOR_0 "STOR <SP> <pathname> <CRLF>   : "
    #define HELP_STOR_1 "Upload file from client to server\r\n"
    #define HELP_STOR   HELP_STOR_0 HELP_STOR_1

    #define HELP_LIST_0 "LIST [<SP> <pathname>] <CRLF> : "
    #define HELP_LIST_1 "List files in the current working directory\r\n"
    #define HELP_LIST   HELP_LIST_0 HELP_LIST_1

#endif /*HELP_H*/
