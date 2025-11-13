#include "command.h"
#include "my_ftp.h"

#include <stdlib.h>
#include <unistd.h>

int quit(ftp_t *ftp, int index, char *command)
{
    if (!(ftp->client[index - 1].connection & CONNECTED))
        return EXIT_FAILURE;
    /*IF NO ARG write("502")*/
    write(ftp->polling.fds[index].fd, "500\r\n", 5);
    ftp->polling.fds[index].fd = -1;
    ftp->polling.fds[index].revents = 0;
    return EXIT_FAILURE;
}