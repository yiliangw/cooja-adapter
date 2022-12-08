/*
 * Cooja adapter libevent backend
 */
#include "event-internal.h"
#include "rtimer.h"
#include "util-internal.h"
#include "log-internal.h"

#ifdef PLATFORM_NATIVE
#include <unistd.h>
#elif defined(PLATFORM_COOJA)
#include "sys/cooja_mt.h"
#include "lib/simEnvChange.h"
#endif

static void *coojaev_init(struct event_base *);
static int coojaev_add(struct event_base *, int, short old, short events, void*);
static int coojaev_del(struct event_base *, int, short old, short events, void*);
static int coojaev_dispatch(struct event_base *, struct timeval *);

const struct eventop coojaaops = {
	"select",
	coojaev_init,
	coojaev_add,
	coojaev_del,
	coojaev_dispatch,
	0,  // TODO:
};


struct coojaaop {
	int i;
};

static void *coojaev_init(struct event_base *base)
{
	struct coojaaop *cop;

	if (!(cop = mm_calloc(1, sizeof(struct coojaaop))))
		return NULL;

	return cop;
}

static int
coojaev_add(struct event_base *base, int fd, short old, short events, void *p)
{
	return 0;
}

static int coojaev_del(struct event_base *base , int fd, short old, short events, void*p)
{
	return 0;
}

extern rtimer_clock_t simRtimerCurrentTicks;
static int coojaev_dispatch(struct event_base *base, struct timeval *tv)
{
    #ifdef PLATFORM_NATIVE
		if (tv != NULL) {
			sleep((unsigned int) tv->tv_sec);
		}
	#elif defined(PLATFORM_COOJA)
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

		/* Simulate the elapsed time after the interrupt of the timer. */
		simRtimerCurrentTicks += 100;
	#endif

	return 0;
}
