#include "platform/event2.h"

#include "platform/dev/radio.h"

#include <unistd.h>

int platform_coojaa_dispatch(struct event_base *base, struct timeval *tv)
{
    if (tv != NULL)
        sleep((unsigned int) tv->tv_sec);

    return 0;
}
