#include <cstdlib>

/* prefix all C header files from coojaa with `extern "C" */
extern "C" {
#include <string.h>
#include <stdio.h>  /* We should use <stdio.h> rather than <iostream> for logging in cooja simulation */
#include "coojaa/event2/util.h"
#include "coojaa/event2/event.h"
#include "coojaa/event2/event_struct.h"
#include "coojaa/sys/socket.h"
#include "lib/assert.h"

#include "coojaa/node-id.h"
}

static void timeout_cb(evutil_socket_t fd, short event, void *arg);
static void receive_cb(evutil_socket_t fd, short event, void *arg);
static void init_sendbuf();
static void send_packet(evutil_socket_t fd, short event, void *arg);

static bool is_sender()
{
    const int id = get_node_id();
    return id == 1 || id == 2;
}

static int radiofd;
static struct event_base *base;

#define PACKET_SIZE    64
#define SEND_BUF_SIZE   PACKET_SIZE
#define RECV_BUF_SIZE   (3 * PACKET_SIZE) 
static unsigned char sendbuf[SEND_BUF_SIZE];
static unsigned char recvbuf[RECV_BUF_SIZE];

/* Define the entry as extern "C"` since its called from C code */
extern "C"
int main()
{
    struct event *ev_timeout, *ev_receive;
    struct timeval tv;

    /* Connect to the radio driver socket */
    radiofd = socket(0, 0, 0);
    if (radiofd == -1) {
        printf("Radio socket open failed. radiofd=%d\n", radiofd);
        return -1;
    }

    if (connect(radiofd, &radio_sockaddr, radio_sockaddr_len)) {
        printf("Radio socket connnect failed.\n");
        return -1;
    }

    /* Initialize the event library */
    base = event_base_new();

    if (is_sender()) {
        /* Timeout event: send packets every 3 seconds. */
        init_sendbuf();
        ev_timeout = event_new(base, -1, EV_PERSIST, timeout_cb, event_self_cbarg());
        evutil_timerclear(&tv);
        tv.tv_usec = 50000;
        event_add(ev_timeout, &tv);
    } else {
        /* Receive event: print the contents of received packets */
        ev_receive = event_new(base, radiofd, EV_PERSIST|EV_READ, receive_cb, event_self_cbarg());
        event_add(ev_receive, NULL);
    }
    
    

    event_base_dispatch(base);

    event_base_free(base);

    return 0;
}

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    struct event *ev_send;

    ev_send = event_new(base, radiofd, EV_WRITE, send_packet, NULL);
    event_add(ev_send, NULL);
}

static void receive_cb(evutil_socket_t fd, short event, void *arg)
{
    ssize_t res = recv(radiofd, recvbuf, RECV_BUF_SIZE, 0);

    if (res == -1)
        printf("Fail to receive\n");
    else {
        printf("Receive: ");
        for (int i = 0; i < res; i++) {
            printf("%u", (unsigned int)recvbuf[i]);
        }
        printf("\n");
    }

}

static void init_sendbuf()
{
    char c = (unsigned char) get_node_id();

    for (int i = 0; i < SEND_BUF_SIZE; i++) {
        sendbuf[i] = c;
    }
}

static void send_packet(evutil_socket_t fd, short event, void *arg)
{
    send(radiofd, sendbuf, SEND_BUF_SIZE, 0);
}
