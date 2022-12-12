#include "platform/event2.h"

#include "coojaa/event2/event.h"
#include "internal/fd.h"
#include "internal/log.h"
#include "platform/cooja_mt.h"
#include "platform/simEnvChange.h"
#include "platform/dev/radio.h"
#include "sys/rtimer.h"

extern rtimer_clock_t simRtimerCurrentTicks;
int platform_coojaa_dispatch(struct event_base *base, struct timeval *tv)
{
    struct coojaaop *cop = base->evbase;
    int radio_res = 0;

    /* Notice cooja to shcedule next wake up. */
    if (tv != NULL) {
        rtimer_clock_t t = timeval_to_clocktime(tv) + rtimer_arch_now() + 100;
        rtimer_arch_schedule(t);
    }

    simProcessRunValue = base->event_count_active;

    if (simDontFallAsleep) {
        simDontFallAsleep = 0;
        simProcessRunValue = 1;
    }

    event_msgx("(%s) Before yield: simProcessRunValue = %d", __func__, simProcessRunValue);
    cooja_mt_yield(); /* Return to COOJA */

    event_msgx("(%s) After yield", __func__);

    /* Check whether radio is available */
    int radio_idx = fd_to_index(RADIO_FD);
    
    if (cop->fd_in[radio_idx] && radio_read_prepared())
        radio_res |= EV_READ;

    if (cop->fd_out[radio_idx] && radio_send_prepared())
        radio_res |= EV_WRITE;
    
    if (radio_res)
        evmap_io_active_(base, RADIO_FD, radio_res);

    // /* Simulate the elapsed time after the interrupts (timer, radio, etc.). */
    // simRtimerCurrentTicks += 100;

    return 0;
}