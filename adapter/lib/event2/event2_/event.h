#ifndef EVENT2_EVENT_H_INCLUDED_
#define EVENT2_EVENT_H_INCLUDED_

#ifdef CONTIKI_TARGET_NATIVE
#define PLATFORM_NATIVE
#elif defined(CONTIKI_TARGET_COOJA)
#define PLATFORM_COOJA
#endif

#include "event2_/util.h"
#include "event2_/event_struct.h"

#define EVENT_MAX_PRIORITIES 256

#define EV_TIMEOUT	0x01
#define EV_READ		0x02
#define EV_WRITE	0x04
#define EV_SIGNAL	0x08
#define EV_PERSIST	0x10
#define EV_ET		0x20
#define EV_FINALIZE     0x40
#define EV_CLOSED	0x80

#define EVLOOP_ONCE	0x01
#define EVLOOP_NONBLOCK	0x02
#define EVLOOP_NO_EXIT_ON_EMPTY 0x04

enum event_base_config_flag {
	EVENT_BASE_FLAG_NOLOCK = 0x01,
	// EVENT_BASE_FLAG_IGNORE_ENV = 0x02,
	// EVENT_BASE_FLAG_STARTUP_IOCP = 0x04,
	EVENT_BASE_FLAG_NO_CACHE_TIME = 0x08,
	EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST = 0x10,
	EVENT_BASE_FLAG_PRECISE_TIMER = 0x20
};

struct event_base;

struct event_base *event_base_new(void);
struct event_config *event_config_new(void);
void event_config_free(struct event_config *cfg);
struct event_base *event_base_new_with_config(const struct event_config *cfg);
int event_base_priority_init(struct event_base *base, int npriorities);
int event_base_dispatch(struct event_base *event_base);
int event_base_loop(struct event_base *, int);
void event_base_free(struct event_base *);
int event_assign(struct event *ev, struct event_base *base, evutil_socket_t fd, short events, 
    void (*callback)(evutil_socket_t, short, void *), void *arg);
int event_add(struct event *ev, const struct timeval *tv);

#endif /* EVENT2_EVENT_H_INCLUDED_ */