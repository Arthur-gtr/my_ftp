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

#define EXIT_SUCSESS 0
#define EXIT_FAIL 1

int test(void)
{
    int server_fd;
    int client_fd;
    struct sockaddr_in addr;/*IPV4 type*/
    socklen_t addrlen = sizeof(addr);
    
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }
    addr.sin_family = AF_INET;/*AF_INET sert a indiqué le type IPV4*/
    addr.sin_addr.s_addr = INADDR_ANY;/*Connection a toute les interfaces locale ou pas*/
    addr.sin_port = htons(8080);/*Port d' écoute*/
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {/*Cast du type IPv4 En structure généraliser*/
        perror("bind");
        close(server_fd);
        return 1;
    }
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        return 1;
    }
    struct pollfd fds[10];
    int nfds = 1;
    
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    printf("Serveur en attente de connexion...\n");
    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);/*Cast du type IPv4 En structure généraliser*/
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return 1;
    }
    printf("Client connecté\n");
    int value = 0;
    if (send(client_fd, &value, sizeof(value), 0) < 0) {
        perror("send");
    }
    close(client_fd);
    close(server_fd);
    return 0;
}

int reterr(char *str)
{
    write(2, str, strlen(str));
    write(2, "\n", 1);
    return EXIT_FAILURE;
}
/*
Boucle Poll pour pouvoir gardé lire tout en meme temps
*/

/*INIT the server and fill the serv structure*/
int init_server(server_t *serv, int port)
{
    serv->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv->server_fd != 0)
        return reterr("Socket build error");
    serv->addr.sin_family = AF_INET;/*AF_INET sert a indiqué le type IPV4*/
    serv->addr.sin_addr.s_addr = INADDR_ANY;/*Connection a toute les interfaces locale ou pas*/
    serv->addr.sin_port = htons(8080);/*Port d' écoute*//*A remplacer par la variable port*/
    return EXIT_SUCCESS;
}

/*INIT all element in the ftp struct*/
int init_ftp(ftp_t *ftp)
{
    if (fill_server(&ftp->server) == EXIT_FAILURE)
        return EXIT_FAILURE;
    ftp->client = NULL;
    return EXIT_SUCCESS;
}

int my_ftp()
{
    ftp_t ftp;
    if (init_ftp(&ftp) == EXIT_FAILURE)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

int main(int ac, char **av)
{
    return my_ftp();
}