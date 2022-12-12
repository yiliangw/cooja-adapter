#ifndef EVENT_FRAMWORK_H_
#define EVENT_FRAMWORK_H_

#include "internal/clock.h"
#include "platform-conf.h"

struct event_framework {
    int (*timeout_pending) (void);
    clock_time_t (*timeout_next) (void);
};

extern struct event_framework libevent_evf;

#define EVENT_FRAMWORK() libevent_evf


#endif /* EVENT_FRAMWORK_H_ */