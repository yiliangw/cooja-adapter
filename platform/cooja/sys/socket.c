#include "coojaa/sys/socket.h"
#include "coojaa/dev/radio.h"
#include <errno.h>

#define RADIO_SOCKET ((int)1209)

int connect(int fd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (fd == RADIO_SOCKET)
        return 0;
    
    errno = EACCES;
    return -1;
}

int socket(int domain, int type, int protocol)
{
    return RADIO_SOCKET;
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
    int res;

    if (sockfd != RADIO_SOCKET) {
        errno = EACCES;
        goto err;
    }

    res = radio_driver.send(buf, len);
    if (res != RADIO_TX_OK) {
        errno = EIO;
        goto err;
    }

    return len;

err:
    return -1;
}

ssize_t recv(int sockfd, void *buf, size_t len, int flags)
{
    int res;

    if (sockfd != RADIO_SOCKET) {
        errno = EACCES;
        goto err;
    }

    res = radio_driver.read(buf, len);
    if (res == 0 && len != 0) {
        errno = EIO;
        goto err;
    }
    
    return res;

err:
    return -1;
}