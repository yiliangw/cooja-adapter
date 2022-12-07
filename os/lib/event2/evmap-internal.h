#ifndef EVMAP_INTERNAL_H_INCLUDED_
#define EVMAP_INTERNAL_H_INCLUDED_

#include "event2_/util.h"

struct event_base;
struct event;

int evmap_io_add_(struct event_base *base, evutil_socket_t fd, struct event *ev);
int evmap_io_del_(struct event_base *base, evutil_socket_t fd, struct event *ev);
int evmap_signal_add_(struct event_base *base, int sig, struct event *ev);
int evmap_signal_del_(struct event_base *base, int sig, struct event *ev);

#endif /* EVMAP_INTERNAL_H_INCLUDED_ */
