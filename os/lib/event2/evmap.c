#include "evmap-internal.h"
#include "event2_/util.h"
#include "event2_/event.h"
#include "event2_/event_struct.h"
#include "event-internal.h"
#include "util-internal.h"
#include "log-internal.h"
#include "mm-internal.h"

#include <limits.h>
#include <string.h>
#include <sys/queue.h>

/** An entry for an evmap_io list: notes all the events that want to read or
	write on a given fd, and the number of each.
  */
struct evmap_io {
	struct event_dlist events;
	ev_uint16_t nread;
	ev_uint16_t nwrite;
	ev_uint16_t nclose;
};

/* An entry for an evmap_signal list: notes all the events that want to know
   when a signal triggers. */
struct evmap_signal {
	struct event_dlist events;
};

/* Set the variable 'x' to the field in event_map 'map' with fields of type
   'struct type *' corresponding to the fd or signal 'slot'.  Set 'x' to NULL
   if there are no entries for 'slot'.  Does no bounds-checking. */
#define GET_SIGNAL_SLOT(x, map, slot, type)			\
	(x) = (struct type *)((map)->entries[slot])
/* As GET_SLOT, but construct the entry for 'slot' if it is not present,
   by allocating enough memory for a 'struct type', and initializing the new
   value by calling the function 'ctor' on it.  Makes the function
   return -1 on allocation failure.
 */
#define GET_SIGNAL_SLOT_AND_CTOR(x, map, slot, type, ctor, fdinfo_len)	\
	do {								\
		if ((map)->entries[slot] == NULL) {			\
			(map)->entries[slot] =				\
			    mm_calloc(1,sizeof(struct type)+fdinfo_len); \
			if (EVUTIL_UNLIKELY((map)->entries[slot] == NULL)) \
				return (-1);				\
			(ctor)((struct type *)(map)->entries[slot]);	\
		}							\
		(x) = (struct type *)((map)->entries[slot]);		\
	} while (0)

#define GET_IO_SLOT(x,map,slot,type) GET_SIGNAL_SLOT(x,map,slot,type)
#define GET_IO_SLOT_AND_CTOR(x,map,slot,type,ctor,fdinfo_len)	\
	GET_SIGNAL_SLOT_AND_CTOR(x,map,slot,type,ctor,fdinfo_len)
#define FDINFO_OFFSET sizeof(struct evmap_io)

static int evmap_make_space(struct event_signal_map *map, int slot, int msize);
static void evmap_io_init(struct evmap_io *entry);
static void evmap_signal_init(struct evmap_signal *entry);

/* return -1 on error, 0 on success if nothing changed in the event backend,
 * and 1 on success if something did. */
int
evmap_io_add_(struct event_base *base, evutil_socket_t fd, struct event *ev)
{
	const struct eventop *evsel = base->evsel;
	struct event_io_map *io = &base->io;
	struct evmap_io *ctx = NULL;
	int nread, nwrite, nclose, retval = 0;
	short res = 0, old = 0;

	EVUTIL_ASSERT(fd == ev->ev_fd);

	if (fd < 0)
		return 0;

	if (fd >= io->nentries) {
		if (evmap_make_space(io, fd, sizeof(struct evmap_io *)) == -1)
			return (-1);
	}
	GET_IO_SLOT_AND_CTOR(ctx, io, fd, evmap_io, evmap_io_init,
						 evsel->fdinfo_len);

	nread = ctx->nread;
	nwrite = ctx->nwrite;
	nclose = ctx->nclose;

	if (nread)
		old |= EV_READ;
	if (nwrite)
		old |= EV_WRITE;
	if (nclose)
		old |= EV_CLOSED;

	if (ev->ev_events & EV_READ) {
		if (++nread == 1)
			res |= EV_READ;
	}
	if (ev->ev_events & EV_WRITE) {
		if (++nwrite == 1)
			res |= EV_WRITE;
	}
	if (ev->ev_events & EV_CLOSED) {
		if (++nclose == 1)
			res |= EV_CLOSED;
	}
	if (EVUTIL_UNLIKELY(nread > 0xffff || nwrite > 0xffff || nclose > 0xffff)) {
		event_warnx("Too many events reading or writing on fd %d",
		    (int)fd);
		return -1;
	}

	if (res) {
		void *extra = ((char*)ctx) + sizeof(struct evmap_io);
		/* XXX(niels): we cannot mix edge-triggered and
		 * level-triggered, we should probably assert on
		 * this. */
		if (evsel->add(base, ev->ev_fd,
			old, (ev->ev_events & EV_ET) | res, extra) == -1)
			return (-1);
		retval = 1;
	}

	ctx->nread = (ev_uint16_t) nread;
	ctx->nwrite = (ev_uint16_t) nwrite;
	ctx->nclose = (ev_uint16_t) nclose;
	LIST_INSERT_HEAD(&ctx->events, ev, ev_io_next);

	return (retval);
}

/* return -1 on error, 0 on success if nothing changed in the event backend,
 * and 1 on success if something did. */
int
evmap_io_del_(struct event_base *base, evutil_socket_t fd, struct event *ev)
{
	const struct eventop *evsel = base->evsel;
	struct event_io_map *io = &base->io;
	struct evmap_io *ctx;
	int nread, nwrite, nclose, retval = 0;
	short res = 0, old = 0;

	if (fd < 0)
		return 0;

	EVUTIL_ASSERT(fd == ev->ev_fd);

	if (fd >= io->nentries)
		return (-1);

	GET_IO_SLOT(ctx, io, fd, evmap_io);

	nread = ctx->nread;
	nwrite = ctx->nwrite;
	nclose = ctx->nclose;

	if (nread)
		old |= EV_READ;
	if (nwrite)
		old |= EV_WRITE;
	if (nclose)
		old |= EV_CLOSED;

	if (ev->ev_events & EV_READ) {
		if (--nread == 0)
			res |= EV_READ;
		EVUTIL_ASSERT(nread >= 0);
	}
	if (ev->ev_events & EV_WRITE) {
		if (--nwrite == 0)
			res |= EV_WRITE;
		EVUTIL_ASSERT(nwrite >= 0);
	}
	if (ev->ev_events & EV_CLOSED) {
		if (--nclose == 0)
			res |= EV_CLOSED;
		EVUTIL_ASSERT(nclose >= 0);
	}

	if (res) {
		void *extra = ((char*)ctx) + sizeof(struct evmap_io);
		if (evsel->del(base, ev->ev_fd,
			old, (ev->ev_events & EV_ET) | res, extra) == -1) {
			retval = -1;
		} else {
			retval = 1;
		}
	}

	ctx->nread = nread;
	ctx->nwrite = nwrite;
	ctx->nclose = nclose;
	LIST_REMOVE(ev, ev_io_next);

	return (retval);
}

#ifndef NSIG
#define NSIG INT_MAX	// So that we do not need to include <signal.h>
#endif

int
evmap_signal_add_(struct event_base *base, int sig, struct event *ev)
{
	const struct eventop *evsel = base->evsigsel;
	struct event_signal_map *map = &base->sigmap;
	struct evmap_signal *ctx = NULL;

	if (sig < 0 || sig >= NSIG)
		return (-1);

	if (sig >= map->nentries) {
		if (evmap_make_space(
			map, sig, sizeof(struct evmap_signal *)) == -1)
			return (-1);
	}
	GET_SIGNAL_SLOT_AND_CTOR(ctx, map, sig, evmap_signal, evmap_signal_init,
	    base->evsigsel->fdinfo_len);

	if (LIST_EMPTY(&ctx->events)) {
		if (evsel->add(base, ev->ev_fd, 0, EV_SIGNAL, NULL)
		    == -1)
			return (-1);
	}

	LIST_INSERT_HEAD(&ctx->events, ev, ev_signal_next);

	return (1);
}

int
evmap_signal_del_(struct event_base *base, int sig, struct event *ev)
{
	const struct eventop *evsel = base->evsigsel;
	struct event_signal_map *map = &base->sigmap;
	struct evmap_signal *ctx;

	if (sig < 0 || sig >= map->nentries)
		return (-1);

	GET_SIGNAL_SLOT(ctx, map, sig, evmap_signal);

	LIST_REMOVE(ev, ev_signal_next);

	if (LIST_FIRST(&ctx->events) == NULL) {
		if (evsel->del(base, ev->ev_fd, 0, EV_SIGNAL, NULL) == -1)
			return (-1);
	}

	return (1);
}

/** Expand 'map' with new entries of width 'msize' until it is big enough
	to store a value in 'slot'.
 */
static int
evmap_make_space(struct event_signal_map *map, int slot, int msize)
{
	if (map->nentries <= slot) {
		int nentries = map->nentries ? map->nentries : 32;
		void **tmp;

		if (slot > INT_MAX / 2)
			return (-1);

		while (nentries <= slot)
			nentries <<= 1;

		if (nentries > INT_MAX / msize)
			return (-1);

		tmp = (void **)mm_realloc(map->entries, nentries * msize);
		if (tmp == NULL)
			return (-1);

		memset(&tmp[map->nentries], 0,
		    (nentries - map->nentries) * msize);

		map->nentries = nentries;
		map->entries = tmp;
	}

	return (0);
}

static void
evmap_io_init(struct evmap_io *entry)
{
	LIST_INIT(&entry->events);
	entry->nread = 0;
	entry->nwrite = 0;
	entry->nclose = 0;
}

static void evmap_signal_init(struct evmap_signal *entry)
{
	LIST_INIT(&entry->events);
}

void
evmap_signal_initmap_(struct event_signal_map *ctx)
{
	ctx->nentries = 0;
	ctx->entries = NULL;
}

void
evmap_io_initmap_(struct event_io_map* ctx)
{
	evmap_signal_initmap_(ctx);
}

void
event_changelist_init_(struct event_changelist *changelist)
{
	changelist->changes = NULL;
	changelist->changes_size = 0;
	changelist->n_changes = 0;
}