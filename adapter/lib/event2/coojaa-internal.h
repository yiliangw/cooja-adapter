#ifndef COOJAA_INTERNAL_H_INCLUDED_
#define COOJAA_INTERNAL_H_INCLUDED_

#include <stdbool.h>
#include "internal/fd.h"

struct coojaaop {
	bool socket_in[COOJAA_FD_NUM];	/* Whether the sockets have been registerd for read */
	bool socket_out[COOJAA_FD_NUM];	/* Whether the sockets have been registerd for write */
};

static inline int fd_to_index_(int fd)
{
	if (!valid_coojaa_fd(fd))
		return -1;
	
	return fd - COOJAA_FD_FLOOR - 1;
}

static inline int index_to_fd_(int index)
{
    if (index < 0 || index >= COOJAA_FD_NUM)
        return -1;

    return index + COOJAA_FD_FLOOR + 1;
}

#endif /* COOJAA_INTERNAL_H_INCLUDED_ */