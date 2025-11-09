/*
** EPITECH PROJECT, 2025
** my_ftp
** File description:
** run_ftp
*/

#include <poll.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "my_ftp.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <time.h>


/*
Faut d' abords faire une recherche pour savoir ou il faut remplacé pour les gens qui se déconnectes
*/
static
int add_user(ftp_t *ftp)
{   
    client_t *client = NULL;
    struct pollfd *fds = NULL;

    ftp->client->size++;
    ftp->polling.nfds++;
    if (ftp->polling.nfds >= (int)ftp->polling.alloc_pollfd){
        ftp->polling.alloc_pollfd *= 2;
        fds = realloc(ftp->polling.fds, sizeof(struct pollfd) * ftp->polling.alloc_pollfd);
        if (fds == NULL) {
            free(ftp->polling.fds);
            return EXIT_FAILURE;
        }
        ftp->polling.fds = fds;
    }
    if (ftp->client->size >= ftp->client->alloc_client){
        ftp->client->alloc_client *= 2;
        client = realloc(ftp->client, sizeof(client_t) * ftp->client->alloc_client);
        if (client == NULL) {
            free(ftp->client);
            return EXIT_FAILURE;
        }
        ftp->client = client;
    }
    ftp->client[ftp->client->size - 1].addrlen = sizeof(ftp->client[0].addr);
    ftp->client[ftp->client->size - 1].connection = 0;
    ftp->polling.fds[ftp->client->size].fd = accept(ftp->server.server_fd, (struct sockaddr *)&ftp->client[ftp->client->size - 1].addr, &ftp->client[ftp->client->size - 1].addrlen);
    ftp->polling.fds[ftp->client->size].events = POLLIN;
    if (ftp->polling.fds[ftp->client->size].fd == -1)
        return reterr("Error file descriptor");
    write(ftp->polling.fds[ftp->client->size].fd, "220 Connection:\r\n", 17);
    printf("User add to the queu, IP: %s\n", inet_ntoa(ftp->client[ftp->client->size - 1].addr.sin_addr));
    return EXIT_SUCCESS;
}

int get_data(ftp_t *ftp, int index)
{
    char buffer[1024];
    int status = 0;

    status = read(ftp->polling.fds[index].fd, buffer, 1024);
    
    printf("status %d\n", status);
    if (status == -1)
        return EXIT_FAILURE;
    if (status == 0){
        printf("Socket Closed: %d\n", index);
        close(ftp->polling.fds[index].fd);
        ftp->polling.fds[index].fd = -1;
        return EXIT_FAILURE;
    }
    buffer[status] = '\0';
    printf("data %s\n", buffer);
    command_parsing(ftp, index, buffer);
    return EXIT_SUCCESS;
}

static
int check_event(ftp_t *ftp)
{
    if (ftp->polling.fds[FDS_SERVER].revents & POLLIN){
        if (add_user(ftp) == EXIT_FAILURE)
            return reterr("Malloc failed.");
        return EXIT_SUCCESS;
    }
    for (int i = CLIENT_ID_MIN; i < ftp->polling.nfds; i++){
        if (ftp->polling.fds[i].revents & POLLIN){
            //printf("Got Pollin Flag %d\n", ftp->polling.fds[i].revents);
            get_data(ftp, i);

        }
        /*Recevoir des données et déterminer si je dois faires des choses ?*/
        if (ftp->polling.fds[i].revents & (POLLHUP | POLLERR | POLLNVAL)){
            ftp->polling.fds[i].fd = -1;
            ftp->polling.fds[i].revents = 0;
            printf("Deconect\n");
        }
        /*MEttre le fd du fds a -1 pour le supprimé du tableau.*/
    }
    return EXIT_SUCCESS;
}

int run_ftp(ftp_t *ftp)
{
    int status;

    while (ftp->running == RUNNING){
        status = poll(ftp->polling.fds, ftp->polling.nfds, TIMEOUT);
        //printf("Poll status; %d\n", status);
        if (status == 0){
            printf("Timeout\n");
            continue;
        }
        if (status < 0)
            return reterr("Poll Error");
        if (check_event(ftp) == EXIT_FAILURE)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
