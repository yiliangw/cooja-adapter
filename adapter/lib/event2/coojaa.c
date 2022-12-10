/*
 * Cooja adapter libevent backend
 */
#include "event-internal.h"
#include "sys/rtimer.h"
#include "util-internal.h"
#include "log-internal.h"

#include "platform/event2.h"

static void *coojaa_init(struct event_base *);
static int coojaa_add(struct event_base *, int, short old, short events, void*);
static int coojaa_del(struct event_base *, int, short old, short events, void*);
static int coojaa_dispatch(struct event_base *, struct timeval *);

const struct eventop coojaaops = {
	"select",
	coojaa_init,
	coojaa_add,
	coojaa_del,
	coojaa_dispatch,
	0,  // TODO:
};

struct coojaaop {
	int i;
};

static void *coojaa_init(struct event_base *base)
{
	struct coojaaop *cop;

	if (!(cop = mm_calloc(1, sizeof(struct coojaaop))))
		return NULL;

	return cop;
}

static int
coojaa_add(struct event_base *base, int fd, short old, short events, void *p)
{
	return 0;
}

static int coojaa_del(struct event_base *base , int fd, short old, short events, void*p)
{
	return 0;
}

static int coojaa_dispatch(struct event_base *base, struct timeval *tv)
{
	return platform_coojaa_dispatch(base, tv);		
}
