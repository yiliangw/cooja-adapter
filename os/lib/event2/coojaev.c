/*
 * Cooja adapter libevent backend
 */
#include "event-internal.h"
#include <unistd.h>

#ifdef PLATFORM_NATIVE
#include <unistd.h>
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

static int coojaev_dispatch(struct event_base *base, struct timeval *tv)
{
    #ifdef PLATFORM_NATIVE
		sleep((unsigned int) tv->tv_sec);
	#endif
	return 0;
}
