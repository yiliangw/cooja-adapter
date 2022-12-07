#include <stdio.h>
#include <event2_/event.h>

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    static int i = 0;
    printf("Timeout %d\n", i++);
}

int umain()
{
    struct event_base *base;
    struct event timeout;
    struct timeval tv;
    int flags = EV_PERSIST;

    /* Initialize the event library */
    base = event_base_new();

    /* Initialize one event */
	event_assign(&timeout, base, -1, flags, timeout_cb, (void*) &timeout);

    printf("ev->base: %p\n", timeout.ev_base);
    evutil_timerclear(&tv);
	tv.tv_sec = 2;
	event_add(&timeout, &tv);

    event_base_dispatch(base);

    printf("done\n");

    event_base_free(base);

    return 0;
}
