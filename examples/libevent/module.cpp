#include <cstdlib>
#include <string.h>

/* prefix all C header files from coojaa with `extern "C" */
extern "C" {
#include <stdio.h>  /* We should use <stdio.h> rather than <iostream> for logging in cooja simulation */
#include "coojaa/event2/util.h"
#include "coojaa/event2/event.h"
#include "coojaa/event2/event_struct.h"
#include "coojaa/sys/socket.h"
#include "lib/assert.h"
}

struct send_info {
    void *buf;
    int len;
};

static inline struct send_info *new_send_info(int len)
{
    struct send_info *info = (struct send_info*)calloc(1, sizeof(struct send_info));
    assert(info != NULL);
    info->len = len;
    info->buf = calloc(1, len);
    return info;
}

static inline void free_send_info(struct send_info *info)
{
    free(info->buf);
    free(info);
}

static void timeout_cb(evutil_socket_t fd, short event, void *arg);
static void receive_cb(evutil_socket_t fd, short event, void *arg);
static void send_packet(evutil_socket_t fd, short event, void *arg);

static int radiofd;
static struct event_base *base;

#define RECV_BUF_SIZE  64
static char recvbuf[RECV_BUF_SIZE];

/* Define the entry as extern "C"` since its called from C code */
extern "C"
int main()
{
    struct event *ev_timeout, *ev_receive;
    struct timeval tv;

    /* Connect to the radio driver socket */
    radiofd = socket(0, 0, 0);
    if (radiofd == -1) {
        printf("Radio socket connection failed. radiofd=%d\n", radiofd);
        return -1;
    }
    
    /* Initialize the event library */
    base = event_base_new();

    /* Timeout event: send packets every 3 seconds. */
    ev_timeout = event_new(base, -1, EV_PERSIST, timeout_cb, event_self_cbarg());
    evutil_timerclear(&tv);
    tv.tv_sec = 3;
    event_add(ev_timeout, &tv);
    
    /* Receive event: print the contents of received packets */
    ev_receive = event_new(base, radiofd, EV_PERSIST|EV_READ, receive_cb, event_self_cbarg());
    event_add(ev_receive, NULL);

    event_base_dispatch(base);

    event_base_free(base);

    return 0;
}

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    static int cnt = 0;
    static const int buflen = 128;

    struct timeval tv;
    struct event *ev_send;
    struct send_info *info;

    /* Send a packet containing the counter and current time.*/
    info = new_send_info(buflen);
    evutil_gettimeofday(&tv, NULL);
    sprintf((char*)(info->buf), "Periodic packet %d at %ld s\n", cnt, tv.tv_sec);
    
    /* Send the packet once the radio becomes available. */
    ev_send = event_new(base, radiofd, EV_WRITE, send_packet, info);
    event_add(ev_send, NULL);

    cnt++;
}

static void receive_cb(evutil_socket_t fd, short event, void *arg)
{
    memset(recvbuf, 0, RECV_BUF_SIZE);
    ssize_t res = recv(radiofd, recvbuf, RECV_BUF_SIZE, 0);

    if (res == -1)
        printf("%s: Fail to receive\n", __func__);
    else
        printf("%s: Recieve: %s", __func__, recvbuf);

}

static void send_packet(evutil_socket_t fd, short event, void *arg)
{
    struct send_info *info = (struct send_info *)arg;
    send(radiofd, info->buf, info->len, 0);

    /* Free the buffer */
    free_send_info(info);
}
