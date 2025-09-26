#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

/*int get_stdin(char **buffer)
{
    size_t bufsize = 32;
    int characters;

    if (buffer == NULL)
        return -1;
    if (isatty(0) != 0)
        write(1, "$> ", 3);
    free(*buffer);
    *buffer = NULL;
    characters = getline(buffer, &bufsize, stdin);
    if (characters == -1)
        return -1;
    (*buffer)[characters - 1] = '\0';
    return characters;
}

int main(void)
{
    char *buffer = NULL;
    get_stdin(&buffer);
    printf("%s\n", buffer);
    get_stdin(&buffer);
    free(buffer);
}*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(void)
{
    int sockfd;
    struct sockaddr_in serv_addr;

    // Création du socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); // même port que le serveur
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // serveur local

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        return 1;
    }

    printf("Connecté au serveur.\n");

    // Réception de la valeur envoyée par le serveur
    int value;
    if (recv(sockfd, &value, sizeof(value), 0) < 0) {
        perror("recv");
        close(sockfd);
        return 1;
    }

    printf("Reçu du serveur : %d\n", value);

    close(sockfd);
    return 0;
}
