/*
 * CoojaA libevent backend
 */
#include "coojaa/event2/event.h"
#include "coojaa/sys/socket.h"
#include "event-internal.h"
#include "sys/rtimer.h"
#include "util-internal.h"
#include "log-internal.h"
#include "coojaa-internal.h"

#include "platform/event2.h"
#include <asm-generic/errno-base.h>
#include <errno.h>

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
	struct coojaaop *cop = base->evbase;
	int idx = fd_to_index_(fd);

	(void) p;

	if (idx == -1) {
		event_warn("Tried to add an invalid fd: %d\n", fd);
		errno = EACCES;
		return -1;
	}

	if (events & EV_READ)
		cop->socket_in[idx] = true;

	if (events & EV_WRITE)
		cop->socket_in[idx] = true;

	return 0;
}

static int coojaa_del(struct event_base *base , int fd, short old, short events, void*p)
{
	struct coojaaop *cop = base->evbase;
	int idx = fd_to_index_(fd);

	(void) p;

	if (idx == -1) {
		event_warn("Tried to add an invalid fd: %d\n", fd);
		return 0;
	}

	if (events & EV_READ)
		cop->socket_in[idx] = false;
		
	if (events & EV_WRITE)
		cop->socket_in[idx] = false;

	return 0;
}

static int coojaa_dispatch(struct event_base *base, struct timeval *tv)
{
	return platform_coojaa_dispatch(base, tv);		
}
