#ifndef COOJAA_SYS_SOCKET_H
#define COOJAA_SYS_SOCKET_H

#include <bits/sockaddr.h>
#include <sys/types.h>

enum coojaa_scoket {
  COOJAA_SOCKET_MIN = 100,
  RADIO_SOCKET,
  COOJAA_SOCKET_MAX
};

#define COOJAA_SOCKET_NUM (COOJAA_SOCKET_MAX - COOJAA_SOCKET_MIN - 1)

static inline int valid_coojaa_socket(int fd)
{
  return (fd > COOJAA_SOCKET_MIN) && (fd < COOJAA_SOCKET_MAX); 
}

struct sockaddr
  {
    __SOCKADDR_COMMON (sa_);	/* Common data: address family and length.  */
    char sa_data[14];		/* Address data.  */
  };

typedef unsigned int socklen_t;

int connect(int fd, const struct sockaddr *addr, socklen_t len);
int socket(int domain, int type, int protocol);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

#endif /* COOJAA_SYS_SOCKET_H */