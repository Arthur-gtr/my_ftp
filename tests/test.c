#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "my_ftp.h"
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
        if(!isdigit(buffer[i]) && is_int == true){
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

int fill_ip(char ip[32], char arg[2048])
{
    int size = 0;
    int pos = 0;

    for(int i = 0; i != 4 ; i++){
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

int fill_sysport(char *port, const char *src, int pos)
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

int get_port(int pos, const char *src)
{
    char port1[4] = {0};
    char port2[4] = {0};
    int p1 = 0;
    int p2 = 0;

    pos = fill_sysport(port1, src, pos);
    if (pos == -1)
        return pos;
    if (src[pos] != ',')
        return -1;
    pos++;
    pos = fill_sysport(port2, src, pos);
    if (pos == -1)
        return pos;
    p1 = atoi(port1);
    p2 = atoi(port2);
    if ((p2 > 255 || p2 < 0) || (p1 > 255 || p1 < 0))
        return -1;
    return p1 * 256 + p2;
}

int fill_port(client_t *client, char arg[2048])
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
}


/*

1 9 2 . 1 6 8 . 1 . 2
                     
0 1 2 3 4 5 6 7 8 9 10

*/
void main(void)
{
    char ip[32] = {0};
    char arg[2048] = "192,168,1,2,8,255";

    if (fill_port(NULL, arg) == -1)
        printf("Error\n");
}