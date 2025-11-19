/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** port
*/

#include "my_ftp.h"
#include "command.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include <arpa/inet.h>

int size_n_nb(int n, const char *buffer)
{
    int count = 0;
    int size_int = 0;
    bool is_int = false;

    for (int i = 0; buffer[i] != 0; i++){
        if (count > n)
            return size_int;
        if (isdigit(buffer[i]) && is_int == false)
            is_int = true;
        if (isdigit(buffer[i]) && count == n){
            size_int++;
        }
        if (!isdigit(buffer[i]) && is_int == true){
            count++;
            is_int = false;
        }
    }
    return (size_int > 0) ? size_int : -1;
}

void replace_char(char *src, const char c, const char new_c)
{
    for (int i = 0; src[i] != '\0'; i++)
        if (src[i] == c)
            src[i] = new_c;
}

static
int fill_ip(char ip[32], char arg[2048])
{
    int size = 0;
    int pos = 0;

    for (int i = 0; i != 4; i++){
        size = size_n_nb(i, arg);
        pos += size;
        if (size == -1 || size > 3)
            return -1;
        strncpy(ip, arg, pos);
        pos++;
        if (arg[pos - 1] != ',' && arg[pos - 1] != '\0')
            return -1;
    }
    if (arg[pos - 1] != ',')
        return -1;
    replace_char(ip, ',', '.');
    return pos;
}

static
int fill_port(char *port, const char *src, int pos)
{
    if (!isdigit(src[pos]))
        return -1;
    for (int i = 0; i != 3; i++){
        if (!isdigit(src[pos]))
            break;
        port[i] = src[pos];
        pos++;
    }
    return pos;
}

static
int get_port(int pos, const char *src)
{
    char port1[4] = {0};
    char port2[4] = {0};
    int p1 = 0;
    int p2 = 0;

    pos = fill_port(port1, src, pos);
    if (pos == -1)
        return pos;
    if (src[pos] != ',')
        return -1;
    pos++;
    pos = fill_port(port2, src, pos);
    if (pos == -1)
        return pos;
    p1 = atoi(port1);
    p2 = atoi(port2);
    if ((p2 > 255 || p2 < 0) || (p1 > 255 || p1 < 0))
        return -1;
    return p1 * 256 + p2;
}

int init_sockadrr(client_t *client, char arg[2048])
{
    char ip[32] = {0};
    int sysport = 0;
    int pos = 0;

    pos = fill_ip(ip, arg);
    if (pos == -1)
        return -1;
    sysport = get_port(pos, arg);
    if (sysport == -1)
        return -1;
    client->addr_port.sin_family = AF_INET;
    client->addr_port.sin_port = htons(sysport);
    inet_pton(AF_INET, ip, &client->addr_port.sin_addr);
    return 0;
}

int init_port(ftp_t *ftp, int index, char *command)
{
    if (is_connected(&ftp->client[CLIENT_IDX(index)],
        ftp->polling.fds[index].fd) == false)
        return EXIT_FAILURE;
    if (get_number_arg(command) > 2){
        dprintf(ftp->polling.fds[index].fd, ARG_501);
        ftp->client[CLIENT_IDX(index)].datatransfer_ready = false;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int port(ftp_t *ftp, int index, char *command)
{
    char arg[DATA_BUFFER] = {0};

    if (init_port(ftp, index, command) == EXIT_FAILURE)
        return EXIT_SUCCESS;
    get_n_arg(command, arg, 2);
    if (init_sockadrr(&ftp->client[CLIENT_IDX(index)], arg) == -1){
        ftp->client[CLIENT_IDX(index)].datatransfer_ready = false;
        return EXIT_SUCCESS;
    }
    if (ftp->client[CLIENT_IDX(index)].datatransfer_mode == PORT)
        close(ftp->client[CLIENT_IDX(index)].pasv_fd);
    ftp->client[CLIENT_IDX(index)].datatransfer_ready = true;
    ftp->client[CLIENT_IDX(index)].datatransfer_mode = PORT;
    dprintf(ftp->polling.fds[index].fd, "200 PORT command successful.\r\n");
    return EXIT_SUCCESS;
}
