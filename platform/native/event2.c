#include "platform/event2.h"
#include "platform/dev/radio.h"
#include "coojaa/sys/socket.h"
#include "sys/cc.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/select.h>


int platform_coojaa_dispatch(struct event_base *base, struct timeval *tv)
{
    static char stdinbuf[FOO_RADIO_BUFSIZE];
    
    struct coojaaop *cop = base->evbase;
    fd_set outset, inset;
    int res;
    const int radioidx = fd_to_index_(RADIO_FD);
    const int maxfd = MAX(STDIN_FILENO, STDOUT_FILENO);
    
    FD_ZERO(&outset);
    FD_ZERO(&inset);

    /* We use stdin and stdout to simulate radio input and output */
    if (cop->socket_in[radioidx])   
        FD_SET(STDIN_FILENO, &inset);

    if (cop->socket_out[radioidx])
        FD_SET(STDOUT_FILENO, &outset);

    select(maxfd+1, &inset, &outset, NULL, tv);

    /* Check stdin and stout for foo radio. */
    res = 0;
    if (FD_ISSET(STDIN_FILENO, &inset)) {
        /* A foo radio packet received */
        memset(stdinbuf, 0, FOO_RADIO_BUFSIZE);
        assert(fgets(stdinbuf, FOO_RADIO_BUFSIZE, stdin) != NULL);
        foo_radio_new_packet(stdinbuf, FOO_RADIO_BUFSIZE);
        res |= EV_READ;
    }
    if (FD_ISSET(STDOUT_FILENO, &outset)) {
        foo_radio_send_available();
        res |= EV_WRITE;
    }
    if (res)
        evmap_io_active_(base, RADIO_FD, res);

    return 0;
}
