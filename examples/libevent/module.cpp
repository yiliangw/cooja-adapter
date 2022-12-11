#include <cstdlib>
#include <string.h>

/* prefix all C header files from coojaa with `extern "C" */
extern "C" {
#include <stdio.h>  /* We should use <stdio.h> rather than <iostream> for logging in cooja simulation */
#include "coojaa/event2/util.h"
#include "coojaa/event2/event.h"
#include "coojaa/event2/event_struct.h"
#include "coojaa/sys/socket.h"
}

static void timeout_cb(evutil_socket_t fd, short event, void *arg);
static void receive_cb(evutil_socket_t fd, short event, void *arg);
// static void send_cb(evutil_socket_t fd, short event, void *arg);

static int radiofd;
static struct event_base *base;

#define BUFFER_LEN  64

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

    /* Timeout event */
    // ev_timeout = event_new(base, -1, EV_PERSIST, timeout_cb, event_self_cbarg());
    ev_timeout = event_new(base, -1, 0, timeout_cb, event_self_cbarg());
    evutil_timerclear(&tv);
    tv.tv_sec = 1;
    event_add(ev_timeout, &tv);
    
    /* Receive event */
    ev_receive = event_new(base, radiofd, EV_PERSIST|EV_READ, receive_cb, event_self_cbarg());
    event_add(ev_receive, NULL);

    event_base_dispatch(base);

    event_base_free(base);

    return 0;
}

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    static int cnt = 0;

    struct timeval tv;
    /* Print current time */
    evutil_gettimeofday(&tv, NULL);
    printf("%s: Send message %d at: %ld s\n", __func__, cnt, tv.tv_sec);

    cnt++;
}

static void receive_cb(evutil_socket_t fd, short event, void *arg)
{
    char buf[BUFFER_LEN];
    memset(buf, 0, BUFFER_LEN);
    ssize_t res = recv(radiofd, buf, BUFFER_LEN, 0);

    if (res == -1)
        printf("%s: Fail to receive\n", __func__);
    else
        printf("%s: Recieve: %s", __func__, buf);

}

// static void send_cb(evutil_socket_t fd, short event, void *arg)
// {
//     void *buf = arg;
//     ssize_t res = send(radio_sock, buf, BUFFER_LEN, 0);

//     if (res == -1)
//         printf("%s: Fail to send: %s\n", __func__, (char*)buf);
//     else
//         printf("%s: Sent: %s\n", __func__, (char*)buf);

//     /* Free the buffer */
//     free(buf);
// }
