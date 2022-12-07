#ifndef EVENT2_EVENT_H_INCLUDED_
#define EVENT2_EVENT_H_INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif

#include "event2_/util.h"
#include "event2_/event_struct.h"

/**
 * @name event flags
 *
 * Flags to pass to event_new(), event_assign(), event_pending(), and
 * anything else with an argument of the form "short events"
 */
/**@{*/
/** Indicates that a timeout has occurred.  It's not necessary to pass
 * this flag to event_for new()/event_assign() to get a timeout. */
#define EV_TIMEOUT	0x01
/** Wait for a socket or FD to become readable */
#define EV_READ		0x02
/** Wait for a socket or FD to become writeable */
#define EV_WRITE	0x04
/** Wait for a POSIX signal to be raised*/
#define EV_SIGNAL	0x08
/**
 * Persistent event: won't get removed automatically when activated.
 *
 * When a persistent event with a timeout becomes activated, its timeout
 * is reset to 0.
 */
#define EV_PERSIST	0x10
/** Select edge-triggered behavior, if supported by the backend. */
#define EV_ET		0x20
/**
 * If this option is provided, then event_del() will not block in one thread
 * while waiting for the event callback to complete in another thread.
 *
 * To use this option safely, you may need to use event_finalize() or
 * event_free_finalize() in order to safely tear down an event in a
 * multithreaded application.  See those functions for more information.
 **/
#define EV_FINALIZE     0x40
/**
 * Detects connection close events.  You can use this to detect when a
 * connection has been closed, without having to read all the pending data
 * from a connection.
 *
 * Not all backends support EV_CLOSED.  To detect or require it, use the
 * feature flag EV_FEATURE_EARLY_CLOSE.
 **/
#define EV_CLOSED	0x80
/**@}*/

/* Event loop flags */
/** Block until we have an active event, then exit once all active events
 * have had their callbacks run. */
#define EVLOOP_ONCE	0x01
/** Do not block: see which events are ready now, run the callbacks
 * of the highest-priority ones, then exit. */
#define EVLOOP_NONBLOCK	0x02
/** Do not exit the loop because we have no pending events.  Instead, keep
 * running until event_base_loopexit() or event_base_loopbreak() makes us
 * stop.
 */
#define EVLOOP_NO_EXIT_ON_EMPTY 0x04

struct event_base;

struct event_base *event_base_new(void);
int event_base_dispatch(struct event_base *event_base);
int event_base_loop(struct event_base *, int);
void event_base_free(struct event_base *);
int event_assign(struct event *ev, struct event_base *base, evutil_socket_t fd, short events, 
    void (*callback)(evutil_socket_t, short, void *), void *arg);
int event_add(struct event *ev, const struct timeval *tv);

#ifdef __cplusplus
}
#endif

#endif /* EVENT2_EVENT_H_INCLUDED_ */