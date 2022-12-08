#include "event2_/util.h"
#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <event2_/event.h>

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    struct timeval tv;
    evutil_gettimeofday(&tv, NULL);
    printf("Time: %ld s\n", tv.tv_sec);
}

int main()
{
    struct event_base *base;
    struct event timeout;
    struct timeval tv;
    int flags = EV_PERSIST;

    /* Initialize the event library */
    base = event_base_new();

    /* Initialize one event */
	event_assign(&timeout, base, -1, flags, timeout_cb, (void*) &timeout);

    evutil_timerclear(&tv);
	tv.tv_sec = 1;
	event_add(&timeout, &tv);

    event_base_dispatch(base);

    printf("done\n");

    event_base_free(base);

    return 0;
}
