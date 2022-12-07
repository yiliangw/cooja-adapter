/*
 * Cooja adapter libevent backend
 */
#include "event-internal.h"

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

static int
coojaev_add(struct event_base *base, int fd, short old, short events, void *p)
{

}

static int coojaev_dispatch(struct event_base *, struct timeval *)
{
    
}
