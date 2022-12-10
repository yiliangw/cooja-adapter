#include "platform/event2.h"

#include "../../adapter/lib/event2/event-internal.h"
#include "../../adapter/lib/event2/util-internal.h"
#include "../../adapter/lib/event2/log-internal.h"

#include <unistd.h>

int platform_coojaa_dispatch(struct event_base *base, struct timeval *tv)
{
    if (tv != NULL)
        sleep((unsigned int) tv->tv_sec);

    return 0;
}
