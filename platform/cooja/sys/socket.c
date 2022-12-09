#include "sys/socket.h"
#include "sys/errno.h"
#include "dev/cooja-radio.h"
#include "errno.h"

static const int radio_sock = 1209;

int connect(int fd, const struct sockaddr *addr, socklen_t addrlen)
{
    if (fd == radio_sock)
        return 0;
    
    errno = EACCES;
    return -1;
}

int socket(int domain, int type, int protocol)
{
    return radio_sock;
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
    int res;

    if (sockfd != radio_sock) {
        errno = EACCES;
        goto err;
    }

    res = cooja_radio_driver.send(buf, len);
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

    if (sockfd != radio_sock) {
        errno = EACCES;
        goto err;
    }

    res = cooja_radio_driver.read(buf, len);
    if (res == 0 && len != 0) {
        errno = EIO;
        goto err;
    }
    
    return res;

err:
    return -1;
}