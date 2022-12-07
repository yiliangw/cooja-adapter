/*
 * This file is ported from libevent-2.1.12.
 * See https://github.com/libevent/libevent/blob/patches-2.1/event-internal.h for details.
 */

#ifndef EVENT_INTERNAL_H_INCLUDED_
#define EVENT_INTERNAL_H_INCLUDED_

#include "event2/event.h"
#include "event2_/event_struct.h"
#include "minheap-internal.h"
#include "evsignal-internal.h"
#include "evmap-internal.h"

#include <bits/types/struct_timeval.h>
#include <sys/queue.h>

#define ev_signal_next	ev_.ev_signal.ev_signal_next
#define ev_io_next	ev_.ev_io.ev_io_next
#define ev_io_timeout	ev_.ev_io.ev_timeout

/* used only by signals */
#define ev_ncalls	ev_.ev_signal.ev_ncalls
#define ev_pncalls	ev_.ev_signal.ev_pncalls

#define ev_pri ev_evcallback.evcb_pri
#define ev_flags ev_evcallback.evcb_flags
#define ev_closure ev_evcallback.evcb_closure
#define ev_callback ev_evcallback.evcb_cb_union.evcb_callback
#define ev_arg ev_evcallback.evcb_arg

#define EV_CLOSURE_EVENT 0
#define EV_CLOSURE_EVENT_SIGNAL 1
#define EV_CLOSURE_EVENT_PERSIST 2
#define EV_CLOSURE_CB_SELF 3
#define EV_CLOSURE_CB_FINALIZE 4
#define EV_CLOSURE_EVENT_FINALIZE 5
#define EV_CLOSURE_EVENT_FINALIZE_FREE 6

#define EV_SOCK_FMT "%d"
#define EV_SOCK_ARG(x) (x)

struct eventop {
	const char *name;
	int (*add)(struct event_base *, evutil_socket_t fd, short old, short events, void *fdinfo);
	int (*del)(struct event_base *, evutil_socket_t fd, short old, short events, void *fdinfo);
	int (*dispatch)(struct event_base *, struct timeval *);
	size_t fdinfo_len;
};

struct event_changelist {
	struct event_change *changes;
	int n_changes;
	int changes_size;
};

struct event_signal_map {
	void **entries;
	int nentries;
};

#define event_io_map event_signal_map

struct common_timeout_list {
	struct event_list events;
	struct timeval duration;
	struct event timeout_event;
	struct event_base *base;
};

TAILQ_HEAD(evcallback_list, event_callback);

struct event_base {
	const struct eventop *evsel;
	void *evbase;
	struct event_changelist changelist;

	const struct eventop *evsigsel;
	struct evsig_info sig;

	enum event_base_config_flag flags;

	int running_loop;
	int virtual_event_count;
	int virtual_event_count_max;

	int event_count;
	int event_count_max;
	int event_count_active;
	int event_count_active_max;
	int event_gotterm;
	int event_break;
	int event_continue;

	int n_deferreds_queued;

	struct common_timeout_list **common_timeout_queues;
	int n_common_timeouts;
	int n_common_timeouts_allocated;
	int event_running_priority;
	struct event_io_map io;
	struct event_signal_map sigmap;
	struct min_heap timeheap;
	struct evcallback_list *activequeues;
	int nactivequeues;
	struct evcallback_list active_later_queue;
	struct timeval max_dispatch_time;
	int max_dispatch_callbacks;
	int limit_callbacks_after_prio;
	struct event_callback *current_event;
};

struct event_config {
	TAILQ_HEAD(event_configq, event_config_entry) entries;

	int n_cpus_hint;
	struct timeval max_dispatch_interval;
	int max_dispatch_callbacks;
	int limit_callbacks_after_prio;
	enum event_method_feature require_features;
	enum event_base_config_flag flags;
};

#define N_ACTIVE_CALLBACKS(base)					\
	((base)->event_count_active)

#define EVENT_DEL_NOBLOCK 0
#define EVENT_DEL_BLOCK 1
#define EVENT_DEL_AUTOBLOCK 2
#define EVENT_DEL_EVEN_IF_FINALIZING 3

int event_del_nolock_(struct event *ev, int blocking);

void event_active_nolock_(struct event *ev, int res, short ncalls);
int event_callback_activate_nolock_(struct event_base *base, struct event_callback *evcb);

int event_add_nolock_(struct event *ev, const struct timeval *tv, int tv_is_absolute);

#endif /* EVENT_INTERNAL_H_INCLUDED_ */