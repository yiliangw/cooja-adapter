#include "platform/event2.h"

#include "../../adapter/lib/event2/event-internal.h"
#include "../../adapter/lib/event2/util-internal.h"
#include "../../adapter/lib/event2/log-internal.h"

#include "coojaa/sys/cooja_mt.h"
#include "sys/rtimer.h"
#include "lib/simEnvChange.h"


extern rtimer_clock_t simRtimerCurrentTicks;
int platform_coojaa_dispatch(struct event_base *base, struct timeval *tv)
{
    if (tv != NULL) {
        rtimer_clock_t t = timeval_to_clocktime(tv) + rtimer_arch_now();
        event_debug(("t = %lu", t));
        rtimer_arch_schedule(t);
    }

    simProcessRunValue = base->event_count_active;

    
    if (simDontFallAsleep) {
        simDontFallAsleep = 0;
        simProcessRunValue = 1;
    }

    /* Return to COOJA */
    cooja_mt_yield();

    /* Simulate the elapsed time after the interrupts (timer, radio, etc.). */
    simRtimerCurrentTicks += 100;

    return 0;
}