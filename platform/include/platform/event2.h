#ifndef COOJAA_PLATFORM_EVENT2_H_
#define COOJAA_PLATFORM_EVENT2_H_

/* Implementation header files of libevent */
#include "../../adapter/lib/event2/event-internal.h"
#include "../../adapter/lib/event2/util-internal.h"
#include "../../adapter/lib/event2/log-internal.h"
#include "../../adapter/lib/event2/coojaa-internal.h"
#include "../../adapter/lib/event2/evmap-internal.h"

/* Functions which should be implemented for every platform */
int platform_coojaa_dispatch(struct event_base *base, struct timeval *tv);

#endif /* COOJAA_PLATFORM_EVENT2_H_ */
