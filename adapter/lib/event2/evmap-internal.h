#ifndef EVMAP_INTERNAL_H_INCLUDED_
#define EVMAP_INTERNAL_H_INCLUDED_

#include "coojaa/event2/event_struct.h"
#include "event-internal.h"

struct event_base;
struct event;

int evmap_io_add_(struct event_base *base, evutil_socket_t fd, struct event *ev);
int evmap_io_del_(struct event_base *base, evutil_socket_t fd, struct event *ev);
int evmap_signal_add_(struct event_base *base, int sig, struct event *ev);
int evmap_signal_del_(struct event_base *base, int sig, struct event *ev);
void evmap_io_initmap_(struct event_io_map *ctx);
void evmap_signal_initmap_(struct event_signal_map *ctx);
void evmap_io_active_(struct event_base *base, evutil_socket_t fd, short events);

#endif /* EVMAP_INTERNAL_H_INCLUDED_ */
