#ifndef COOJAA_SYS_SOCKET_H
#define COOJAA_SYS_SOCKET_H

#include "internal/fd.h"

#include <bits/sockaddr.h>
#include <sys/types.h>

struct sockaddr
  {
    __SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  */
    char sa_data[14];		/* Address data.  */
  };

typedef unsigned int socklen_t;

extern const struct sockaddr radio_sockaddr;
extern const socklen_t radio_sockaddr_len;

int connect(int fd, const struct sockaddr *addr, socklen_t len);
int socket(int domain, int type, int protocol);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

#endif /* COOJAA_SYS_SOCKET_H */