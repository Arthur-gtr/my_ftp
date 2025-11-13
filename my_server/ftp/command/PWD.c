#include "my_ftp.h"

#include <stdlib.h>
#include <stdio.h>

int pwd(ftp_t *ftp, int index, char *command)
{
    printf("257 \"%s\"\r\n",ftp->client[CLIENT_IDX(index)].wd);
    dprintf(ftp->polling.fds[index].fd, "257 \"%s\"\r\n",ftp->client[CLIENT_IDX(index)].wd);
    return EXIT_SUCCESS;
}